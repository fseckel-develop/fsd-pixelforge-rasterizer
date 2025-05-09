#include "Window.h"
#include "Input.h"
#include <iostream>
using namespace std;


bool Input::mouseHasMoved = false;
Position Input::mousePosition = Position{0.0f, 0.0f};
Position Input::mouseDelta = Position{0.0f, 0.0f};
unordered_map<int, bool> Input::mouseButtonStates;
unordered_map<int, bool> Input::keyStates;
float Input::lastTime = 0.0f;
float Input::currentTime = 0.0f;
float Input::deltaTime = 0.0f;


void Input::Enable() {
    glfwSetCursorPosCallback(Window::GetWindow(), MousePositionCallback);
    glfwSetMouseButtonCallback(Window::GetWindow(), MouseButtonCallback);
    glfwSetKeyCallback(Window::GetWindow(), KeyCallback);
    glfwSetInputMode(Window::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    currentTime = static_cast<float>(glfwGetTime());
    lastTime = currentTime;
}


Position Input::GetMousePosition() {
    return mousePosition;
}


Position Input::GetMouseDelta() {
    const Position result = mouseDelta;
    mouseDelta = Position{0.0f, 0.0f};
    return result;
}


float Input::GetDeltaTime() {
    return deltaTime;
}


bool Input::IsMouseButtonPressed(const int button) {
    return mouseButtonStates[button];
}


bool Input::IsKeyPressed(const int key) {
    return keyStates[key];
}


void Input::TimeStep() {
    currentTime = static_cast<float>(glfwGetTime());
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
}



void Input::PollEvents() {
    glfwPollEvents();
}


void Input::MousePositionCallback(GLFWwindow* /*window*/, const double newX, const double newY) {
    if (!mouseHasMoved) {
        mousePosition = Position{newX, newY};
        mouseHasMoved = true;
    }
    mouseDelta.x = newX - mousePosition.x;
    mouseDelta.y = mousePosition.y - newY;
    mousePosition = Position{newX, newY};
}


void Input::MouseButtonCallback(GLFWwindow* /*window*/, const int button, const int action, int /*mods*/) {
    if (action == GLFW_PRESS) {
        mouseButtonStates[button] = true;
    } else if (action == GLFW_RELEASE) {
        mouseButtonStates[button] = false;
    }
    if (button == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(Window::GetWindow(), GLFW_TRUE);
    }
}


void Input::KeyCallback(GLFWwindow* window, const int key, int /*scancode*/, const int action, int /*mods*/) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (action == GLFW_PRESS) {
        keyStates[key] = true;
    } else if (action == GLFW_RELEASE) {
        keyStates[key] = false;
    }
}