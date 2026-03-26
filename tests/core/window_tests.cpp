#include <doctest.h>
#include <pixelforge/core/window.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using pixelforge::core::Window;

extern GLFWwindow* getGLTestContextWindow();

namespace {

    struct WindowGuard {
        ~WindowGuard() {
            if (Window::getWindow() != nullptr) {
                Window::close();
            }
            if (getGLTestContextWindow() != nullptr) {
                glfwMakeContextCurrent(getGLTestContextWindow());
            }
        }
    };

} // namespace


TEST_CASE("Window create creates GLFW window") {
    WindowGuard guard;

    Window::create(640, 480, "Window Test");

    REQUIRE(Window::getWindow() != nullptr);
}

TEST_CASE("Window create initializes framebuffer dimensions") {
    WindowGuard guard;

    Window::create(640, 480, "Window Test");

    REQUIRE(Window::getWindow() != nullptr);
    CHECK(Window::getBufferWidth() > 0);
    CHECK(Window::getBufferHeight() > 0);
}

TEST_CASE("Window getAspectRatio returns positive ratio") {
    WindowGuard guard;

    Window::create(800, 600, "Window Test");

    REQUIRE(Window::getWindow() != nullptr);
    CHECK(Window::getAspectRatio() > 0.0f);
    CHECK(Window::getAspectRatio() == doctest::Approx(
        static_cast<float>(Window::getBufferWidth()) /
        static_cast<float>(Window::getBufferHeight())
    ));
}

TEST_CASE("Window setViewport updates OpenGL viewport to framebuffer size") {
    WindowGuard guard;

    Window::create(640, 480, "Window Test");

    REQUIRE(Window::getWindow() != nullptr);

    Window::setViewport();

    GLint viewport[4] = {0, 0, 0, 0};
    glGetIntegerv(GL_VIEWPORT, viewport);

    CHECK(viewport[0] == 0);
    CHECK(viewport[1] == 0);
    CHECK(viewport[2] == Window::getBufferWidth());
    CHECK(viewport[3] == Window::getBufferHeight());
}

TEST_CASE("Window shouldClose is false immediately after create") {
    WindowGuard guard;

    Window::create(640, 480, "Window Test");

    REQUIRE(Window::getWindow() != nullptr);
    CHECK_FALSE(Window::shouldClose());
}

TEST_CASE("Window shouldClose reflects GLFW close flag") {
    WindowGuard guard;

    Window::create(640, 480, "Window Test");

    REQUIRE(Window::getWindow() != nullptr);

    glfwSetWindowShouldClose(Window::getWindow(), GLFW_TRUE);

    CHECK(Window::shouldClose());
}

TEST_CASE("Window swapBuffers does not produce OpenGL error") {
    WindowGuard guard;

    Window::create(640, 480, "Window Test");

    REQUIRE(Window::getWindow() != nullptr);

    glGetError();
    Window::swapBuffers();
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("Window close destroys window") {
    WindowGuard guard;

    Window::create(640, 480, "Window Test");

    REQUIRE(Window::getWindow() != nullptr);

    Window::close();

    CHECK(Window::getWindow() == nullptr);
}
