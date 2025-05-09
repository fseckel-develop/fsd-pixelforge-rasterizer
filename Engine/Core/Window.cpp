#include "Window.h"
#include <iostream>
using namespace std;


GLFWwindow* Window::window = nullptr;
const char* Window::windowTitle = nullptr;
GLint Window::windowWidth = 0;
GLint Window::windowHeight = 0;
GLint Window::bufferWidth = 0;
GLint Window::bufferHeight = 0;


void Window::Create(const GLint width, const GLint height, const char* title) {
    windowWidth = width;
    windowHeight = height;
    windowTitle = title;
    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr, nullptr);
    if (!window) {
        cerr << "Failed to create GLFW window." << endl;
        return;
    }
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetWindowUserPointer(window, nullptr);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetWindowSizeCallback(window, WindowSizeCallback);
    SetViewport();
}


GLFWwindow* Window::GetWindow() {
    return window;
}


void Window::SwapBuffers() {
    glfwSwapBuffers(window);
}


void Window::SetViewport() {
    glViewport(0, 0, bufferWidth, bufferHeight);
}


bool Window::ShouldClose() {
    return glfwWindowShouldClose(window);
}


GLint Window::GetBufferWidth() {
    return bufferWidth;
}


GLint Window::GetBufferHeight() {
    return bufferHeight;
}


float Window::GetAspectRatio() {
    return static_cast<float>(bufferWidth) / static_cast<float>(bufferHeight);
}


void Window::Close() {
    glfwDestroyWindow(window);
}


void Window::WindowSizeCallback(GLFWwindow* /*window*/, const int newWidth, const int newHeight) {
    windowWidth = newWidth;
    windowHeight = newHeight;
}


void Window::FramebufferSizeCallback(GLFWwindow* /*window*/, const int newBufferWidth, const int newBufferHeight) {
    bufferWidth = newBufferWidth;
    bufferHeight = newBufferHeight;
    SetViewport();
}
