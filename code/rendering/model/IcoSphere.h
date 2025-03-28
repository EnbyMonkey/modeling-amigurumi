#pragma once

#include "defs.h"
#include "model/Model.h"

class Node;
class IcoSphere : public Model {
public:
	IcoSphere(Eigen::Vector4d colour, double size);
    ~IcoSphere();
};