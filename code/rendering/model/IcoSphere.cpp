#include "model/IcoSphere.h"

/**
 * @brief Construct a new Ico Sphere:: Ico Sphere object
 *
 * @param colour
 * @param size
 */
IcoSphere::IcoSphere(Eigen::Vector4d colour, double size)
{
    // set the colour of the icosphere
    ColourList colours;
    colours.push_back(1.0f/255 * colour);

    loadObj("../code/rendering/model/obj/icosphere.obj", colours);

    // set material
    _material.ambient = 2;
    _material.diffuse = 1;
    _material.specular = 0.2;
    _material.radiation = 0.5;

    // set initial model transformation
    scale *= size * 0.5; // divide by 2 to get stitch radius
}

/**
 * @brief Destroy the Ico Sphere:: Ico Sphere object
 *
 */
IcoSphere::~IcoSphere() {}