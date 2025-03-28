#pragma once

#include "defs.h"
#include "Piece.h"

class Project {
public:
    Project(std::string name, float yarn_size, Eigen::Vector4d yarn_colour);
    ~Project();

private:
    std::string name;
    float yarn_size;
    Eigen::Vector4d yarn_colour;
    //attachment/relations
    std::vector<Piece*> piece;
};
