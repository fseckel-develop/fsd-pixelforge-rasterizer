#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest.h>
#include <pixelforge/core/context.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>


namespace {

    GLFWwindow* g_testContextWindow = nullptr;

    class GLTestContext {
    public:
        GLTestContext() {
            pixelforge::core::context::initializeGLFW();
            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
            g_testContextWindow = glfwCreateWindow(32, 32, "pixelforge-tests-gl", nullptr, nullptr);
            if (!g_testContextWindow) {
                glfwTerminate();
                throw std::runtime_error("Failed to create hidden GLFW window.");
            }
            glfwMakeContextCurrent(g_testContextWindow);
            pixelforge::core::context::initializeGLEW();
            glGetError();
        }

        ~GLTestContext() {
            if (g_testContextWindow) {
                glfwDestroyWindow(g_testContextWindow);
                g_testContextWindow = nullptr;
            }
            glfwTerminate();
        }

        GLTestContext(const GLTestContext&) = delete;
        GLTestContext& operator=(const GLTestContext&) = delete;
    };

} // namespace


GLFWwindow* getGLTestContextWindow() {
    return g_testContextWindow;
}


int main(const int argc, char** argv) {
    try {
        GLTestContext context;
        doctest::Context doctestContext;
        doctestContext.applyCommandLine(argc, argv);
        return doctestContext.run();
    } catch (const std::exception& ex) {
        std::cerr << "GL test context initialization failed: " << ex.what() << '\n';
        return EXIT_FAILURE;
    }
}
