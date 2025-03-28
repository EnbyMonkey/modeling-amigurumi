#include <polyscope/polyscope.h> // basic polyscope functionality
#include <polyscope/surface_mesh.h> // for surface mesh (e.g. regular mesh) registration and stuff
#include <Eigen/Core>
#include <gsl/gsl> // guideline specification library

#include <defs.h>
#include <model/IcoSphere.h>
#include <Pattern.h>
#include <graphcontrol.h>

int main() {
    std::cout << "Project: " << PROJECT_NAME << ", version "
              << PROJECT_VERSION_MAJOR << "."
              << PROJECT_VERSION_MINOR << std::endl;





    // setup polyscope
    polyscope::options::programName = "Amigurumi designer";
    polyscope::options::ssaaFactor = 2; // i don't remember what this is for

    // set scene parameters
    polyscope::options::autocenterStructures = true;
    polyscope::options::autoscaleStructures = true;
    polyscope::options::automaticallyComputeSceneExtents = false;
    polyscope::state::lengthScale = 10.;
    polyscope::state::boundingBox = std::tuple<glm::vec3, glm::vec3>{ {-1., -1., -1.}, {1., 1., 1.} };

    // remove default ground plane
    polyscope::options::groundPlaneMode = polyscope::GroundPlaneMode::None;

    // initialize polyscope
    polyscope::init();

    // test pattern parsing
    Pattern* pattern = new Pattern();
    pattern->init_pattern();

    // test graph building
    GraphControl* gc = new GraphControl();
    gc->init_graph(pattern);

    IcoSphere temp(Eigen::Vector4d{0, 0, 0, 0}, 1);

    Eigen::MatrixXd vertices(42, 3);
    Eigen::MatrixXi faces(80, 3);

    // setup vertices
    auto model_vertices = temp.getVertices();
    for (gsl::index i = 0; i < vertices.rows(); ++i) {

        vertices(i, 0) = model_vertices[i].pos(0);
        vertices(i, 1) = model_vertices[i].pos(1);
        vertices(i, 2) = model_vertices[i].pos(2);
    }

    auto model_faces = temp.getIndices();
    for (gsl::index i = 0; i < faces.rows(); ++i) {
        faces(i, 0) = model_faces[3*i];
        faces(i, 1) = model_faces[3*i+1];
        faces(i, 2) = model_faces[3*i+2];
    }


    // create a sphere for each node
    Graph* g = gc->get_graph();
    for (const auto& id : g->get_nodes()) {
        auto mesh = polyscope::registerSurfaceMesh("node_" + std::to_string(id), vertices, faces);
        auto position = g->get_node(id).position;
        mesh->setPosition(glm::vec3 (position[0], position[1], position[2]));
        mesh->setSurfaceColor((1.f/255.f)*glm::vec3(37, 150, 190));
        //mesh->setSurfaceColor((1.f/255.f)*glm::vec3(5*id, 5*id, 5*id));
    }


    polyscope::show();



    return 0;
}