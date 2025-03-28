#pragma once

#include "defs.h"
#include "Pattern.h"

class Piece {
public:
  Piece(std::string name, float yarn_size, Eigen::Vector4d yarn_colour, Pattern* pattern = new Pattern());
  ~Piece();

private:
    std::string name;
    float yarn_size;
    Eigen::Vector4d yarn_colour;
    Eigen::Vector3d position;
    //attachment points
    Pattern* pattern;
};
