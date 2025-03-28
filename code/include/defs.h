#pragma once

#include <gsl/gsl> // guideline specification library
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

// eigen3 library for math
#include <Eigen/Core>


// include a config file created by cmake
#include "config.h" // this file is in the build directory


enum class Action_type {
    // stitch actions
    sc,
    sc_back, // Second node of inc
    // non-stitch actions
    sc_forward, // Second part of dec (no node)
    mc, // invisible node
    fo // invisible node
};


/*
inc -> sc sc_back
dec -> sc sc_forward
*/