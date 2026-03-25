#include <pixelforge/core/window.hpp>
#include <iostream>
#include "core/input.hpp"


namespace pixelforge::core {

    bool Input::mouseHasMoved_ = false;
    Position Input::mousePosition_ = Position{0.0f, 0.0f};
    Position Input::mouseDelta_ = Position{0.0f, 0.0f};
    std::unordered_map<int, bool> Input::mouseButtonStates_;
    std::unordered_map<int, bool> Input::keyStates_;
    float Input::lastTime_ = 0.0f;
    float Input::currentTime_ = 0.0f;
    float Input::timeDelta_ = 0.0f;


    void Input::enable() {
        glfwSetCursorPosCallback(Window::getWindow(), mousePositionCallback);
        glfwSetMouseButtonCallback(Window::getWindow(), mouseButtonCallback);
        glfwSetKeyCallback(Window::getWindow(), keyCallback);
        glfwSetInputMode(Window::getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        currentTime_ = static_cast<float>(glfwGetTime());
        lastTime_ = currentTime_;
    }


    Position Input::getMousePosition() {
        return mousePosition_;
    }


    Position Input::getMouseDelta() {
        const Position result = mouseDelta_;
        mouseDelta_ = Position{0.0f, 0.0f};
        return result;
    }


    float Input::getTimeDelta() {
        return timeDelta_;
    }


    bool Input::isMouseButtonPressed(const int button) {
        return mouseButtonStates_[button];
    }


    bool Input::isKeyPressed(const int key) {
        return keyStates_[key];
    }


    void Input::timeStep() {
        currentTime_ = static_cast<float>(glfwGetTime());
        timeDelta_ = currentTime_ - lastTime_;
        lastTime_ = currentTime_;
    }



    void Input::pollEvents() {
        glfwPollEvents();
    }


    void Input::mousePositionCallback(GLFWwindow* /*window*/, const double newX, const double newY) {
        if (!mouseHasMoved_) {
            mousePosition_ = Position{newX, newY};
            mouseHasMoved_ = true;
        }
        mouseDelta_.x = newX - mousePosition_.x;
        mouseDelta_.y = mousePosition_.y - newY;
        mousePosition_ = Position{newX, newY};
    }


    void Input::mouseButtonCallback(GLFWwindow* /*window*/, const int button, const int action, int /*mods*/) {
        if (action == GLFW_PRESS) {
            mouseButtonStates_[button] = true;
        } else if (action == GLFW_RELEASE) {
            mouseButtonStates_[button] = false;
        }
        if (button == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(Window::getWindow(), GLFW_TRUE);
        }
    }


    void Input::keyCallback(GLFWwindow* window, const int key, int /*scancode*/, const int action, int /*mods*/) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        if (action == GLFW_PRESS) {
            keyStates_[key] = true;
        } else if (action == GLFW_RELEASE) {
            keyStates_[key] = false;
        }
    }

} // namespace pixelforge::core
