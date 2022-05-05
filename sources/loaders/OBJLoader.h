//
// Created by Владимир Шелестов aka DarthShelL on 05.05.2022.
//

#ifndef GRAPHICS_OBJLOADER_H
#define GRAPHICS_OBJLOADER_H

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <vector>
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class OBJLoader {
public:
    static void
    Load(const char* filename, vector<float> &vertices, vector<float> &normals, vector<GLushort> &elements);
};


#endif //GRAPHICS_OBJLOADER_H
