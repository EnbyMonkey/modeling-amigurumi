#pragma once

// this header is the intellectual property of us.

#include "defs.h"


struct material {
    float ambient;
    float diffuse;
    float specular;
    float radiation;
};


struct Vertex {
    Vertex() {}

    //posiiton, colour, normal, & uv
    Vertex(const Eigen::Vector4d pos, const Eigen::Vector4d colour, const Eigen::Vector3d normal, const Eigen::Vector2d uv) {
        this->pos = pos;
        this->colour = colour;
        this->normal = normal;
        this->uv = uv;
    }

    Eigen::Vector4d pos;
    Eigen::Vector3d normal;
    Eigen::Vector4d colour;
    Eigen::Vector2d uv;
};


typedef std::vector<Vertex> Vertices;
typedef std::vector<unsigned int> Indices;

typedef std::vector<Eigen::Vector4d> ColourList;


class Model {
public:
	Model();
	~Model();


    void loadObj(std::string filePath, ColourList colours);

	Eigen::Matrix4d getModelMat();
    material getMaterial();
    Eigen::Vector3d getScale();
    Eigen::Vector3d getPosition();
    Vertices getVertices();
    Indices getIndices();

    void setScale(Eigen::Vector3d scale);
    void setPosition(Eigen::Vector3d position);

protected:

    // attributes
	Eigen::Matrix4d modelMat;
    material _material;

    Eigen::Vector3d scale; // scale of the model in model space
    Eigen::Vector3d rotation_axis; // axis of rotation (normalized)
    float rotation_angle; // angle of rotation, in degrees
    Eigen::Vector3d position; // position of the model in world space


	Vertices vertices;
	Indices indices;

};
