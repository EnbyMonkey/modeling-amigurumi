#pragma once

#include <graaflib/graph.h>
#include <graaflib/io/dot.h>
#include <Action.h>

#include "defs.h"
#include "ceressolver.h"


using namespace graaf;
typedef vertex_id_t node_id;

struct Node {
    Node(node_id p, node_id w) :
        previous(p),
        working(w) {};
    Node(Node&& n) : // move constructor
        position(std::move(n.position)),
        previous(n.previous),
        working(n.working) {}

    Eigen::Vector3d position;
    node_id previous;
    node_id working;
};


enum class Edge_type {sequential, working, other};

struct Edge : public weighted_edge<double> {
    Edge(double length, Edge_type type, bool flip) :
        length(length),
        type(type),
        flip(flip) {}
    Edge(Edge&& e) :
        length(e.length),
        type(e.type),
        flip(e.flip) {} // move constructor

    [[nodiscard]] double get_weight() const noexcept override { return length; } // this is required by graaflib for weighted edges

    double length;
    Edge_type type;
    bool flip; // this is a temporary solution
};


class Graph {
public:
	Graph();
    ~Graph();

    Node& get_node(node_id);
    Node& get_node(Action*);
    std::unordered_set<node_id> get_neighbours();
    std::vector<node_id> get_nodes(); // can maybe replace this with an iterator

    Node& add_to_graph(Action*);
    void run_ceres(int num_iterations = 50);

    void log_graph();

private:
    node_id add_node(Node&&, Action*); // move the node
    void add_edge(node_id n1, node_id n2, Edge_type, double length); // order is important here. (n1,n2) means n2 is the previous or working node
    void increment_working_node();

    node_id get_previous_node(node_id);
    node_id get_working_node(node_id);
    node_id get_next_node(node_id);
    std::unordered_set<node_id> get_neighbours(node_id);

    undirected_graph<Node, Edge> graph{};
    std::map<Action*, node_id> action_node_map;
    std::map<node_id, Action*> node_action_map;

    // for graph building
    node_id last_node; // maybe rename to previous_node?
    node_id working_node;

};

typedef std::vector<Graph*> Graphs;

// vertex writer needed to generate dot file
const auto vertex_writer{[](graaf::vertex_id_t vertex_id, const Node& node) -> std::string {
    return "Node " + std::to_string(vertex_id) + ": previous = " + std::to_string(node.previous) + ", working: " + std::to_string(node.working);
}};

// edge writer needed to generate dot file
const auto edge_writer{[](const graaf::edge_id_t& edge_id, const auto& edge) -> std::string {
    Edge_type type = edge.type;
    std::string s;
    switch (type) {
        case Edge_type::sequential:
            s = "previous";
            break;
        case Edge_type::working:
            s = "working";
            break;
        case Edge_type::other:
            s = "other";
            break;
    }

    if (edge.flip) {
        s += ", flip";
    }
    return s;
}};