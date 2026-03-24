#include <pixelforge/core/window.hpp>
#include <iostream>


namespace pixelforge::core {

    using std::cerr;
    using std::endl;


    GLFWwindow* Window::window_ = nullptr;
    const char* Window::windowTitle_ = nullptr;
    GLint Window::windowWidth_ = 0;
    GLint Window::windowHeight_ = 0;
    GLint Window::bufferWidth_ = 0;
    GLint Window::bufferHeight_ = 0;


    void Window::create(const GLint width, const GLint height, const char* title) {
        windowWidth_ = width;
        windowHeight_ = height;
        windowTitle_ = title;
        window_ = glfwCreateWindow(windowWidth_, windowHeight_, windowTitle_, nullptr, nullptr);
        if (!window_) {
            cerr << "Failed to create GLFW window." << endl;
            return;
        }
        glfwGetFramebufferSize(window_, &bufferWidth_, &bufferHeight_);
        glfwMakeContextCurrent(window_);
        glfwSwapInterval(1);
        glfwSetWindowUserPointer(window_, nullptr);
        glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);
        glfwSetWindowSizeCallback(window_, windowSizeCallback);
        setViewport();
    }


    GLFWwindow* Window::getWindow() {
        return window_;
    }


    void Window::swapBuffers() {
        glfwSwapBuffers(window_);
    }


    void Window::setViewport() {
        glViewport(0, 0, bufferWidth_, bufferHeight_);
    }


    bool Window::shouldClose() {
        return glfwWindowShouldClose(window_);
    }


    GLint Window::getBufferWidth() {
        return bufferWidth_;
    }


    GLint Window::getBufferHeight() {
        return bufferHeight_;
    }


    float Window::getAspectRatio() {
        return static_cast<float>(bufferWidth_) / static_cast<float>(bufferHeight_);
    }


    void Window::close() {
        glfwDestroyWindow(window_);
    }


    void Window::windowSizeCallback(GLFWwindow* /*window*/, const int newWidth, const int newHeight) {
        windowWidth_ = newWidth;
        windowHeight_ = newHeight;
    }


    void Window::framebufferSizeCallback(GLFWwindow* /*window*/, const int newBufferWidth, const int newBufferHeight) {
        bufferWidth_ = newBufferWidth;
        bufferHeight_ = newBufferHeight;
        setViewport();
    }

} // namespace pixelforge::core
