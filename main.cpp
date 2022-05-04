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
    Shader *colorShader = load_shader("resources/shaders/color.vsh", "resources/shaders/color.fsh");
    if (colorShader == nullptr) {
        std::cerr << "ERROR::MAIN:Shader loading failed" << std::endl;
        Window::Terminate();
        return -1;
    }
    Shader *lightShader = load_shader("resources/shaders/light.vsh", "resources/shaders/light.fsh");
    if (colorShader == nullptr) {
        std::cerr << "ERROR::MAIN:Shader loading failed" << std::endl;
        Window::Terminate();
        return -1;
    }

    // CUBE
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    uint vbo, vao, lightVao; // ebo;

    // Cube
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // light
    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // -----

    // load and create a texture
    // -------------------------
    Texture *texture;
    texture = load_texture("resources/textures/wall.png");


    float lastTime = glfwGetTime();
    float delta = 0.0f;

    // lighting
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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

        // render
        Window::Clear();

        // textures
        texture->bind();

        // shader
        colorShader->Use();
        // ------

        // render boxes
        glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        colorShader->SetVec3f("lightPos", camera.Position);
        colorShader->SetVec3f("objectColor", objectColor);
        colorShader->SetVec3f("lightColor",  lightColor);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
                                                100.0f);
        colorShader->SetMat4f("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        colorShader->SetMat4f("view", view);

        glBindVertexArray(vao);
        for (unsigned int i = 0; i < 10; i++) {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            colorShader->SetMat4f("model", model);
            glm::mat3 ti_model = glm::mat3(glm::transpose(glm::inverse(model)));
            colorShader->SetMat3f("ti_model", ti_model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // render light
        lightShader->Use();
        lightShader->SetMat4f("projection", projection);
        lightShader->SetMat4f("view", view);
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::mat4(1.0f);
        model = glm::translate(model, camera.Position);
        model = glm::scale(model, glm::vec3(0.01f)); // a smaller cube
        lightShader->SetMat4f("model", model);

        glBindVertexArray(lightVao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

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
    delete colorShader;

    Events::Terminate();
    Window::Terminate();
    return 0;
}
