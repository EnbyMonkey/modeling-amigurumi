#pragma once

#include "defs.h"
#include "Action.h"

class Stitch: public Action {
public:
    Stitch(float yarn_size, Eigen::Vector4d yarn_colour);
    ~Stitch();

private:
    float yarn_size;
    Eigen::Vector4d yarn_colour;
};
