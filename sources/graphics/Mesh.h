//
// Created by Владимир Шелестов aka DarthShelL on 22.03.2022.
//

#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

#include <stdlib.h>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "Shader.h"

using namespace glm;
using namespace std;

typedef unsigned int uint;

#define MAX_BONE_INFLUENCE 4

// Vertex data structure
struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

// Texture data structure
struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    // Mesh data
    vector<Vertex> vertices;
    vector<uint> indices;
    vector<Texture> textures;
    unsigned int VAO;

    Mesh(vector<Vertex> vertices, vector<uint> indices, vector<Texture> textures);
    ~Mesh();

    void Draw(Shader &shader);
private:
    // Render data
    uint VBO, EBO;

    void SetupMesh();
};


#endif //GRAPHICS_MESH_H
