//
// Created by Владимир Шелестов aka DarthShelL on 22.03.2022.
//

#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include <string>

typedef unsigned int uint;

class Shader {
public:
    uint id;

    void Use();

    Shader(uint id);
    ~Shader();
};

extern Shader* load_shader(std::string vertexFileName, std::string fragmentFileName);

#endif //GRAPHICS_SHADER_H
