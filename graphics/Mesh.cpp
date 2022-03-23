//
// Created by Владимир Шелестов aka DarthShelL on 22.03.2022.
//

#include "Mesh.h"
#include <glad/glad.h>

Mesh::Mesh(const float *buffer, size_t vertices, const int *attributes) {
    _vertex_size = 0;

    // Calculating vertex size
    for (int i = 0; attributes[i]; i++) {
        _vertex_size += attributes[i];
    }

    // Generating vao & vbo
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    // Bind
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertex_size * _vertices, buffer, GL_STATIC_DRAW);

    // Attributes
    int offset = 0;
    for (int i = 0; attributes[i]; i++) {
        int size = attributes[i];
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, _vertex_size * sizeof(float),
                              (GLvoid *) (offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += size;
    }

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}
