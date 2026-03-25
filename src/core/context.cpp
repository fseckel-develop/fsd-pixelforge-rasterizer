#include <pixelforge/core/context.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


namespace pixelforge::core::context {

    void initializeGLFW() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW.");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    }

    void initializeGLEW() {
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("Failed to initialize GLEW.");
        }
    }

} // namespace pixelforge::core::context
