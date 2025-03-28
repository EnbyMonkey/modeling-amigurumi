#include "Piece.h"

/** Constructor **/
Piece::Piece(std::string name, float yarn_size, Eigen::Vector4d yarn_colour, Pattern* pattern) :
    name(name),
    yarn_size(yarn_size),
    yarn_colour(yarn_colour),
    pattern(pattern),
    position(0, 0, 0)
{

}
Piece::~Piece() {}
