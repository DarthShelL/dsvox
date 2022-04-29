//
// Created by Владимир Шелестов aka DarthShelL on 21.03.2022.
//

#include "window/Window.h"
#include "window/Events.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>




typedef unsigned int uint;
// settings
const uint SCR_WIDTH = 800;
const uint SCR_HEIGHT = 600;


int main() {
    Window::Initialize(SCR_WIDTH, SCR_HEIGHT, "Title");
    Events::Initialize();

    // Shader
    // -----
    Shader* shader = load_shader("resources/shaders/simple.vsh", "resources/shaders/simple.fsh");
    if (shader == nullptr){
        std::cerr << "ERROR::MAIN:Shader loading failed" << std::endl;
        Window::Terminate();
        return -1;
    }

    // SQUARE
    // -----
    float vertices[] = {
            // positions          // colors           // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    uint vbo, vao, ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // -----

    // load and create a texture
    // -------------------------
    Texture* texture;
    texture = load_texture("resources/textures/wall.png");


    float lastTime = glfwGetTime();
    float delta = 0.0f;

    // render loop
    // -----------
    while (!Window::IsWindowShouldClose()) {
        // frame counter
        // -----
        float currentTime = glfwGetTime();
        delta = currentTime - lastTime;
        lastTime = currentTime;

        // input
        // -----
        if (Events::JustPressed(GLFW_KEY_ESCAPE))
            Window::SetWindowShouldClose(true);

        // render
        Window::Clear();

        // textures
        texture->bind();

        // shader
        shader->Use();
        // ------

//        glBindVertexArray(vao);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//        glBindVertexArray(0);

        // polling events
        // ------
        Window::SwapBuffers();
        Events::PollEvents();
    }

    // terminate
    // ------------------------------------------------------------------
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    delete shader;

    Events::Terminate();
    Window::Terminate();
    return 0;
}
