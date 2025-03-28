#include "ceressolver.h"


// setup the ceres solver, parameter blocks, residual blocks, and run the solver
// nodes: a [node_id, position] map
// edges: [node_id, node_id, edge_length] tuples
// neighours: a list of lists, each neighbours[i] contains indices of neighbours of node[i]
// constraint_edges: a list of edge-related information, specifically an index in nodes, a position, and an edge length. The position will be set constant in the parameter block
// max_iterations: the maximum number of iterations to run the solver
// lambda: a parameter to determine the contribution of edge length energy function and curvature energy function
void CeresSolver::run(std::map<size_t, double*>& nodes, std::vector<std::tuple<size_t, size_t, double>>& edges, std::map<size_t, std::vector<size_t>>& neighbours_list, int max_iterations, double lambda_) {
    ceres::Problem problem;

    ceres::ScaledLoss* one_minus_lambda = new ceres::ScaledLoss(nullptr, 1-lambda_, ceres::TAKE_OWNERSHIP);
    ceres::ScaledLoss* lambda = new ceres::ScaledLoss(nullptr, lambda_, ceres::TAKE_OWNERSHIP);


    // create residual blocks for the edge length energy portion of the least squares problem
    for (const auto& edge : edges) {
        ceres::CostFunction* edge_length_energy_function = new ceres::AutoDiffCostFunction<EdgeLengthFunctor, 1, 3, 3>(new EdgeLengthFunctor(std::get<2>(edge)));
        problem.AddResidualBlock(edge_length_energy_function, one_minus_lambda, nodes.at(std::get<0>(edge)), nodes.at(std::get<1>(edge)));
    }


    // create residual blocks for the curvature portion of the system
    // the parameter blocks are stored in 'parameters'
    // each parameter block is a node position (size = 3)
    for (const auto& [node, neighbours] : neighbours_list) {
        if (neighbours.size() == 0) { // node doesn't have any neighbours (should never really happen tho)
            continue;
        }

        auto* curvature_energy_function = new ceres::DynamicAutoDiffCostFunction<CurvatureFunctor, 4>(new CurvatureFunctor(neighbours.size()-1));
        curvature_energy_function->SetNumResiduals(1);

        std::vector<double*> parameters;
        parameters.push_back(nodes[node]); // position of node
        curvature_energy_function->AddParameterBlock(3);
        for (auto& neighbour : neighbours) {
            parameters.push_back(nodes[neighbour]); // position of neighbours
            curvature_energy_function->AddParameterBlock(3);
        }

        problem.AddResidualBlock((ceres::CostFunction*) curvature_energy_function, lambda, parameters);
    }



    ceres::Solver::Options options;
    //options.linear_solver_type = ceres::SPARSE_NORMAL_CHOLESKY;
    options.linear_solver_type = ceres::CGNR;
    options.max_num_iterations = max_iterations;
    options.minimizer_progress_to_stdout = false;
    ceres::Solver::Summary summary;
    Solve(options, &problem, &summary);

    // there's some kind of weird bug with using cout, maybe caused by incorrect libraries being linked
    // would need to look at PATH and stuff but it's confusing and honestly I would prefer to reinstall linux lol
    //std::cout << summary.BriefReport() << "\n";
    //const char* report = summary.BriefReport().c_str();
    //std::printf("%s\n", report);
    std::ofstream out("resources/ceres-summary.log");
    std::cout.rdbuf(out.rdbuf());
    std::cout << summary.FullReport() << "\n";

}