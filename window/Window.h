//
// Created by Владимир Шелестов aka DarthShelL on 21.03.2022.
//

#ifndef WINDOW_WINDOW_H
#define WINDOW_WINDOW_H

#include <iostream>

class GLFWwindow;

class Window {
public:
    static GLFWwindow* window;
    static int Initialize(int width, int height, const char* title);
    static void Terminate();
    static bool IsWindowShouldClose();
    static void SetWindowShouldClose(bool flag);
    static void SwapBuffers();
    static void Clear();
    static void SetClearColor(float r, float g, float b, float a);
};


#endif //WINDOW_WINDOW_H
