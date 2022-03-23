//
// Created by Владимир Шелестов aka DarthShelL on 21.03.2022.
//

#include "window/Window.h"
#include "window/Events.h"
#include "graphics/Shader.h"


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

    // Poligon
    // -----
    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
    };

    uint vbo, vao;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // -----


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
        // ------
        Window::Clear();

        shader->Use();

//        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
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
    delete shader;

    Events::Terminate();
    Window::Terminate();
    return 0;
}
