//
// Created by Владимир Шелестов aka DarthShelL on 21.03.2022.
//

#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* Window::window;

void error_callback(int error, const char* description) {
    std::cerr << "GLFW::error [" << error << "]: " << description << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int Window::Initialize(int width, int height, const char* title) {
    if (!glfwInit())
    {
        std::cerr << "Window::" << __FUNCTION__ << ": " << "GLFW not initialized" << std::endl;
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window)
    {
        Terminate();
        std::cerr << "Window::" << __FUNCTION__ << ": " << "Window not created" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Window::" << __FUNCTION__ << ": " << "GLAD not loaded" << std::endl;
        return -1;
    }

    SetClearColor(0.1f, 0.18f, 0.2, 1);

    return 0;
}

void Window::Terminate() {
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::IsWindowShouldClose() {
    return glfwWindowShouldClose(window);
}

void Window::SetWindowShouldClose(bool flag){
    glfwSetWindowShouldClose(window, flag);
}

void Window::SwapBuffers(){
    glfwSwapBuffers(window);
}

void Window::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::SetClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}
