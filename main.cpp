//
// Created by Владимир Шелестов aka DarthShelL on 21.03.2022.
//

#include "window/Window.h"
#include "window/Events.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/Camera.h"
#include "loaders/OBJLoader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#define DEBUG 1

#ifdef DEBUG
#define LOG(x) std::cout << "LOG: " << x << std::endl
#else
#define LOG(x)
#endif

typedef unsigned int uint;
// settings
const uint SCR_WIDTH = 1024;
const uint SCR_HEIGHT = 768;


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

    //Monkey
    std::vector<float> vertices, normals;
    std::vector<GLushort> elements;

    OBJLoader::Load("resources/models/monkey_ex.obj", vertices, normals, elements);

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

    uint vbo, vao, lightVao, ebo;

    // Cube ----------
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);
    // element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size()*sizeof(GLushort), &elements[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    // light --------
    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
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
        colorShader->SetVec3f("viewPos", camera.Position);
        colorShader->SetVec3f("lightPos", lightPos);
        colorShader->SetVec3f("objectColor", objectColor);
        colorShader->SetVec3f("lightColor", lightColor);

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
            model = glm::scale(model, glm::vec3(1.5f));
            colorShader->SetMat4f("model", model);
            glm::mat3 ti_model = glm::mat3(glm::transpose(glm::inverse(model)));
            colorShader->SetMat3f("ti_model", ti_model);

            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_SHORT, 0);

//            // Index buffer
//            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//
//            // Draw the triangles !
//            glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, (void *) 0);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // render light
        lightShader->Use();
        lightShader->SetMat4f("projection", projection);
        lightShader->SetMat4f("view", view);
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightShader->SetMat4f("model", model);

        glBindVertexArray(lightVao);
        glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_SHORT, 0);
//        glDrawArrays(GL_TRIANGLES, 0, elements.size());

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
    delete colorShader;
    delete lightShader;

    Events::Terminate();
    Window::Terminate();
    return 0;
}
