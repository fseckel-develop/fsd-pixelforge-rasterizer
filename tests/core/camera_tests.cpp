#include <doctest.h>
#include <pixelforge/core/camera.hpp>
#include <pixelforge/core/window.hpp>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

using pixelforge::core::Camera;
using pixelforge::core::Window;

extern GLFWwindow* getGLTestContextWindow();

namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
    }

    void checkMat4Approx(const glm::mat4& actual, const glm::mat4& expected) {
        for (int col = 0; col < 4; ++col) {
            for (int row = 0; row < 4; ++row) {
                CHECK(actual[col][row] == doctest::Approx(expected[col][row]).epsilon(EPS));
            }
        }
    }

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


TEST_CASE("Camera turnOn sets camera position") {
    constexpr glm::vec3 position(1.0f, 2.0f, 3.0f);

    Camera::turnOn(position);

    checkVec3Approx(Camera::getPosition(), position);
}

TEST_CASE("Camera getPosition returns latest turned on position") {
    Camera::turnOn(glm::vec3(-2.5f, 4.0f, 7.25f));

    const glm::vec3 position = Camera::getPosition();

    checkVec3Approx(position, glm::vec3(-2.5f, 4.0f, 7.25f));
}

TEST_CASE("Camera getViewMatrix matches default orientation after turnOn") {
    const glm::vec3 position(0.0f, 0.0f, 5.0f);

    Camera::turnOn(position);

    // Default yaw=-90, pitch=0 => front=(0,0,-1), up=(0,1,0)
    const glm::mat4 expected = glm::lookAt(
        position,
        position + glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    checkMat4Approx(Camera::getViewMatrix(), expected);
}

TEST_CASE("Camera getViewMatrix changes when camera position changes") {
    Camera::turnOn(glm::vec3(0.0f, 0.0f, 0.0f));
    const glm::mat4 firstView = Camera::getViewMatrix();

    Camera::turnOn(glm::vec3(3.0f, 4.0f, 5.0f));
    const glm::mat4 secondView = Camera::getViewMatrix();

    CHECK_FALSE(firstView == secondView);
}

TEST_CASE("Camera getProjectionMatrix matches expected perspective for window aspect ratio") {
    WindowGuard guard;

    Window::create(800, 600, "Camera Projection Test");
    REQUIRE(Window::getWindow() != nullptr);

    Camera::turnOn(glm::vec3(0.0f, 0.0f, 0.0f));

    const glm::mat4 expected = glm::perspective(
        glm::radians(45.0f),
        Window::getAspectRatio(),
        0.1f,
        1000.0f
    );

    checkMat4Approx(Camera::getProjectionMatrix(), expected);
}

TEST_CASE("Camera projection matrix changes with different window aspect ratio") {
    WindowGuard guard;

    Window::create(1200, 600, "Camera Projection Test");
    REQUIRE(Window::getWindow() != nullptr);

    Camera::turnOn(glm::vec3(0.0f, 0.0f, 0.0f));
    const glm::mat4 wideProjection = Camera::getProjectionMatrix();

    Window::close();
    if (getGLTestContextWindow() != nullptr) {
        glfwMakeContextCurrent(getGLTestContextWindow());
    }

    Window::create(600, 1200, "Camera Projection Test");
    REQUIRE(Window::getWindow() != nullptr);

    const glm::mat4 tallProjection = Camera::getProjectionMatrix();

    CHECK_FALSE(wideProjection == tallProjection);
}
