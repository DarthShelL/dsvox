//
// Created by Владимир Шелестов aka DarthShelL on 22.03.2022.
//

#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Shader::Shader(uint id) : id(id) {
}

Shader::~Shader() {
    glDeleteProgram(id);
}

void Shader::Use() {
    glUseProgram(id);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetVec3f(const std::string &name, glm::vec3 value) {
    glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
}

/**
 * Loads shaders from file
 * @param vertexFileName
 * @param fragmentFileName
 * @return Shader object on success, or nullptr on failure
 */
Shader* load_shader(std::string vertexFileName, std::string fragmentFileName) {
    std::string vertex_code, fragment_code;
    std::ifstream vertex_file, fragment_file;

    // Read from files
    try {
        vertex_file.open(vertexFileName);
        if (vertex_file.fail()) {
            std::cerr << "ERROR::SHADER::FILE_OPEN_FAILED:\n" << vertexFileName << std::endl;
            return nullptr;
        }

        fragment_file.open(fragmentFileName);
        std::stringstream vertex_stream, fragment_stream;
        if (fragment_file.fail()) {
            std::cerr << "ERROR::SHADER::FILE_OPEN_FAILED:\n" << fragmentFileName << std::endl;
            return nullptr;
        }

        vertex_stream << vertex_file.rdbuf();
        fragment_stream << fragment_file.rdbuf();

        vertex_file.close();
        fragment_file.close();

        vertex_code = vertex_stream.str();
        fragment_code = fragment_stream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_READ\n"
        << "explanatory string: " << e.what() << '\n'
        << "Error code: " << e.code() << std::endl;
        return nullptr;
    }

    const char* v_code = vertex_code.c_str();
    const char* f_code = fragment_code.c_str();
    char info[512];
    GLint compiled = GL_FALSE;

    // compilation
    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    if (vertexShader) {
        glShaderSource(vertexShader, 1, &v_code, NULL);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            glGetShaderInfoLog(vertexShader, 512, NULL, info);
            std::cerr << "ERROR::SHADER::COMPILATION_FAILED:\n"<< info << std::endl;
            glDeleteShader(vertexShader);
            return nullptr;
        }
    }

    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (fragmentShader) {
        glShaderSource(fragmentShader, 1, &f_code, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, info);
            std::cerr << "ERROR::SHADER::COMPILATION_FAILED:\n"<< info << std::endl;
            glDeleteShader(fragmentShader);
            return nullptr;
        }
    }

    // create program
    const GLuint program = glCreateProgram();
    if (!program) {
        std::cerr << "ERROR::SHADER::PROGRAM_NOT_CREATED" << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return nullptr;
    }

    // linking
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    // free
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        glGetProgramInfoLog(program, 512, NULL, info);
        std::cerr << "ERROR::SHADER::LINK_FAILED:\n"<< info << std::endl;
        return nullptr;
    }

    return new Shader(program);
}