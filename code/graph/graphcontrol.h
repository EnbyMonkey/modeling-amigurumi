#pragma once

#include <defs.h>
#include <Pattern.h>
#include "graph.h"


class GraphControl {
public:
    GraphControl();
    ~GraphControl();

    void init_graph(Pattern*);
    Graph* get_graph(); // for minimal testing, just return the first (aka only) graph

private:
    void build_graph(Graph*, Pattern*);
    void run_ceres(Graph*, int num_iterations);

    Graphs graphs;
    //std::map<Graph*, ceres::Problem*> problems; // a problem is a system and instructions on how to solve that system - for now assume one problem per graph
};