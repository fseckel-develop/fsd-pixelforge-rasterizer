#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Window {
public:
    static void Create(GLint, GLint, const char*);
    static void SwapBuffers();
    static void SetViewport();
    static bool ShouldClose();
    static GLFWwindow* GetWindow();
    static GLint GetBufferWidth();
    static GLint GetBufferHeight();
    static float GetAspectRatio();
    static void Close();

private:
    static GLFWwindow* window;
    static const char* windowTitle;
    static GLint windowWidth, windowHeight;
    static GLint bufferWidth, bufferHeight;
    static void WindowSizeCallback(GLFWwindow*, int, int);
    static void FramebufferSizeCallback(GLFWwindow*, int, int);
};
