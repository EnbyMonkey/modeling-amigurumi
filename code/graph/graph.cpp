#include "graph.h"

Graph::Graph() {}

Graph::~Graph() {}

Node& Graph::add_to_graph(Action* action)
{
    graaf::vertex_id_t new_node = last_node;

    switch (action->get_type()) {
        case Action_type::sc:
            new_node = add_node(Node{last_node, working_node}, action);
            add_edge(new_node, last_node, Edge_type::sequential, 1);
            add_edge(new_node, working_node, Edge_type::working, 1);
            last_node = new_node;
            increment_working_node();
            break;

        case Action_type::sc_back:
            new_node = add_node(Node{last_node, get_working_node(working_node)}, action);
            add_edge(new_node, last_node, Edge_type::sequential, 1);
            add_edge(new_node, get_working_node(last_node), Edge_type::working, 1);
            last_node = new_node;
            break;

        case Action_type::sc_forward:
            add_edge(last_node, working_node, Edge_type::working, 1);
            increment_working_node();
            break;

        case Action_type::mc: // first node in graph
            new_node = add_node(Node{0, 0}, action);
            working_node = new_node;
            last_node = new_node;
            break;

        case Action_type::fo:
            new_node = add_node(Node{last_node, working_node}, action);
            while (working_node != new_node) {
                add_edge(new_node, working_node, Edge_type::working, 1);
                increment_working_node();
            }
            last_node = new_node;
            break;
    }

    return get_node(new_node);
}


void Graph::log_graph()
{
    const std::filesystem::path dof_file_path{"./resources/graph.dot"};
    graaf::io::to_dot(graph, dof_file_path, vertex_writer, edge_writer);
}

node_id Graph::add_node(Node&& node, Action* a)
{
    node_id id = graph.add_vertex(std::move(node));
    node_action_map[id] = a;
    action_node_map[a] = id;

    return id;
}

void Graph::add_edge(node_id n1, node_id n2, Edge_type type, double length)
{
    bool flip = !(n1 < n2);
    graph.add_edge(n1, n2, Edge{length, type, flip});
}


void Graph::increment_working_node()
{
    //todo: check for turning
    working_node = get_next_node(working_node);
}


void Graph::run_ceres(int num_iterations)
{
    // get node positions and edges
    std::map<node_id, double*> nodes;
    std::map<node_id, std::vector<node_id>> neighbours_list;

    for (const auto& [id, _] : node_action_map) {
        // create node-position map
        double* p = new double[3];
        Node& node = get_node(id);
        p[0] = node.position[0];
        p[1] = node.position[1];
        p[2] = node.position[2];
        nodes[id] = p;

        // get edges
        std::vector<size_t> neighbours;
        for (const auto& neighbour : get_neighbours(id)) {
            neighbours.push_back(neighbour);
        }
        neighbours_list[id] = neighbours;
    }

    // get edges
    std::vector<std::tuple<node_id, node_id, double>> edges;
    const std::unordered_map<edge_id_t, Edge, edge_id_hash>& edge_list = graph.get_edges();
    for (const auto& edge : edge_list) {
        edges.push_back(std::tuple(std::get<0>(std::get<0>(edge)), std::get<1>(std::get<0>(edge)), std::get<1>(edge).length));
    }

    CeresSolver::run(nodes, edges, neighbours_list, num_iterations, 0.7);

    // set new position values
    for (const auto& [id, _] : node_action_map) {
        Node& node = get_node(id);
        node.position[0] = nodes[id][0];
        node.position[1] = nodes[id][1];
        node.position[2] = nodes[id][2];
    }
}



Node& Graph::get_node(node_id n)
{
    return graph.get_vertex(n);
}

Node& Graph::get_node(Action* a)
{
    return get_node(action_node_map[a]);
}

std::vector<node_id> Graph::get_nodes()
{
    std::vector<node_id> nodes;
    for (auto [node, _] : node_action_map) {
        nodes.push_back(node);
    }
    return nodes;
}

node_id Graph::get_previous_node(node_id n)
{
    return get_node(n).previous;
}

node_id Graph::get_working_node(node_id n)
{
    return get_node(n).working;
}

node_id Graph::get_next_node(node_id n)
{
    node_id p;
    for (const auto& neighbour : graph.get_neighbors(n)) {
        if (get_node(neighbour).previous == n) {
            p = neighbour;
            break;
        }
    }
    //todo: error handling
    return p;
}

std::unordered_set<node_id> Graph::get_neighbours(node_id id) { return graph.get_neighbors(id); }