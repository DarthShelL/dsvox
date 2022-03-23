//
// Created by Владимир Шелестов aka DarthShelL on 22.03.2022.
//

#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

#include <stdlib.h>

typedef unsigned int uint;

class Mesh {
private:
    uint _vao;
    uint _vbo;
    size_t _vertices;
    size_t _vertex_size;
public:
    Mesh(const float* buffer, size_t vertices, const int* attributes);
    ~Mesh();
};


#endif //GRAPHICS_MESH_H
