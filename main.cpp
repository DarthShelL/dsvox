//
// Created by Владимир Шелестов aka DarthShelL on 21.03.2022.
//

#include "window/Window.h"
#include "window/Events.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/Camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


typedef unsigned int uint;
// settings
const uint SCR_WIDTH = 800;
const uint SCR_HEIGHT = 600;


int main() {
    Window::Initialize(SCR_WIDTH, SCR_HEIGHT, "Title");
    Events::Initialize();

    // camera
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;

    // Shader
    // -----
    Shader *shader = load_shader("resources/shaders/simple.vsh", "resources/shaders/simple.fsh");
    if (shader == nullptr) {
        std::cerr << "ERROR::MAIN:Shader loading failed" << std::endl;
        Window::Terminate();
        return -1;
    }

    // SQUARE
    // -----
//    float vertices[] = {
//            // positions          // colors             // texture coords
//             0.5f, 0.5f, 0.0f,    1.0f, 0.0f, 0.0f,     1.0f, 1.0f, // top right
//             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,     1.0f, 0.0f, // bottom right
//            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,     0.0f, 0.0f, // bottom left
//            -0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 0.0f,     0.0f, 1.0f  // top left
//    };
//    unsigned int indices[] = {
//            0, 1, 3, // first triangle
//            1, 2, 3  // second triangle
//    };

    // CUBE
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    uint vbo, vao;// ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
//    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // indices
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
//    // color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // -----

    // load and create a texture
    // -------------------------
    Texture *texture;
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
        Window::ProcessInput();
        camera.ProcessInput(delta);
//        if (Events::JustPressed(GLFW_KEY_ESCAPE))
//            Window::SetWindowShouldClose(true);

        // render
        Window::Clear();

        // textures
        texture->bind();

        // shader
        shader->Use();
        // ------

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
                                                100.0f);
        shader->SetMat4f("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        shader->SetMat4f("view", view);

//        glBindVertexArray(vao);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // render boxes
        glBindVertexArray(vao);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader->SetMat4f("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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
//    glDeleteBuffers(1, &ebo);
    delete shader;

    Events::Terminate();
    Window::Terminate();
    return 0;
}
