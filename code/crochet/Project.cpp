#include "Project.h"

/** Constructor **/
Project::Project(std::string name, float yarn_size, Eigen::Vector4d yarn_colour) :
    yarn_size(yarn_size),
    yarn_colour(yarn_colour)
{

}
Project::~Project() {}
