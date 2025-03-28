#include "graphcontrol.h"

GraphControl::GraphControl() {}
GraphControl::~GraphControl() {}

void GraphControl::init_graph(Pattern* p)
{
    Graph* g = new Graph();
    graphs.push_back(g);
    build_graph(g, p);
    run_ceres(g, 5000);
}

void GraphControl::build_graph(Graph* g, Pattern* p)
{
    double height = 0;
    for (const auto& row : p->get_rows()) {
        double theta = 2*M_PI / row->get_actions().size();
        double radius = row->get_actions().size() / (2*M_PI); // this should be proportional to overall stitch_size (right now assume stitch_size = 1)
        int stitch_num = 0;

        for (const auto& action : row->get_actions()) {
            Node& n = g->add_to_graph(action);
            n.position = {radius * cos(theta * stitch_num), height, radius * sin(theta * stitch_num)};
            ++stitch_num;
        }

        // run ceres after positioning each row
        run_ceres(g, 10);
        height = g->get_node(row->get_actions()[0]).position[1] + 1; // should be + stitch_size
    }
}


// initial layout only for rounds for now

void GraphControl::run_ceres(Graph* g, int num_iterations) { g->run_ceres(num_iterations); }

Graph* GraphControl::get_graph() { return graphs[0]; }