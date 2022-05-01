//
// Created by Владимир Шелестов aka DarthShelL on 22.03.2022.
//

#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include <string>
#include <glm/vec3.hpp>
#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>

typedef unsigned int uint;

class Shader {
public:
    uint id;

    void Use();
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetVec3f(const std::string &name, glm::vec3 &value);
    void SetMat4f(const std::string &name, glm::mat4 &value);

    Shader(uint id);
    ~Shader();

};

extern Shader* load_shader(std::string vertexFileName, std::string fragmentFileName);

#endif //GRAPHICS_SHADER_H
