#include "model/Model.h"

// tiny obj loader: https://github.com/tinyobjloader/tinyobjloader
#define TINYOBJLOADER_IMPLEMENTATION
#include "model/tiny_obj_loader.h"

Model::Model() :
    scale(Eigen::Vector3d{1, 1, 1}),
    rotation_axis(Eigen::Vector3d{1, 0, 0}),
    rotation_angle(0),
    position(Eigen::Vector3d{0, 0, 0})
{}

Model::~Model() {}


material Model::getMaterial() {
    return _material;
}

Eigen::Vector3d Model::getScale() {
    return scale;
}

Eigen::Vector3d Model::getPosition() {
    return position;
}

Vertices Model::getVertices() {
    return vertices;
}

Indices Model::getIndices() {
    return indices;
}



/** setters **/
void Model::setScale(Eigen::Vector3d scale) {
    this->scale = scale;
}

void Model::setPosition(Eigen::Vector3d position) {
    this->position = position;
}



/**
* Load a .obj file using tinyobj_loader
*/
void Model::loadObj(std::string filePath, ColourList colours) {
    std::string inputfile = filePath;
    tinyobj::ObjReaderConfig reader_config;

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        std::cout << "Error reading " << filePath << std::endl;
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    //resize to number of vertices
    int vtxNum = (int)attrib.vertices.size();
    vertices.resize(vtxNum);

    //keep track of which vertices have already been set
    std::vector<bool> set(vtxNum, false);


    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                int vtxIndex = idx.vertex_index;
                indices.push_back(vtxIndex);

                if(!set[vtxIndex]) { //only set a vertex once
                    set[vtxIndex] = true;

                    //vertex position
                    tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                    tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                    tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

                    //vertex normal
                    tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                    tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                    tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];

                    //vertex texture coordinates
                    tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                    tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];

                    vertices[vtxIndex] = { { vx, vy, vz, 1 }, colours[s], {nx, ny, nz}, {tx, ty} };
                }
            }
            index_offset += fv;
        }
    }
}
