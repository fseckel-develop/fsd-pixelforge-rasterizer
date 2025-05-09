#include "Camera.h"
#include "Input.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <iostream>
using namespace std; using namespace glm;


vec3 Camera::position = vec3(0.0f, 0.0f, 0.0f);
vec3 Camera::front = vec3(0.0f, 0.0f, -1.0f);
vec3 Camera::up = vec3(0.0f);
vec3 Camera::right = vec3(0.0f);
vec3 Camera::worldUp = vec3(0.0f, 1.0f, 0.0f);
GLfloat Camera::yawAngle = -90.0f;
GLfloat Camera::pitchAngle = 0.0f;
GLfloat Camera::fovAngle = 45.0f;
GLfloat Camera::movementSpeed = 5.0f;
GLfloat Camera::turnSpeed = 0.1f;


void Camera::TurnOn(const vec3 position) {
    Camera::position = position;
    AdjustOrientation();
}


void Camera::Update() {
    HandleKeyInput();
    HandleMouseInput();
    AdjustOrientation();
}


mat4 Camera::GetViewMatrix() {
    return lookAt(position, position + front, up);
}


mat4 Camera::GetProjectionMatrix() {
    return perspective(radians(fovAngle), Window::GetAspectRatio(), 0.1f, 1000.0f);
}


vec3 Camera::GetPosition() {
    return position;
}


void Camera::HandleKeyInput() {
    const GLfloat stride = movementSpeed * Input::GetDeltaTime();
    if (Input::IsKeyPressed(GLFW_KEY_W)) { position += front * stride; }
    if (Input::IsKeyPressed(GLFW_KEY_S)) { position -= front * stride; }
    if (Input::IsKeyPressed(GLFW_KEY_D)) { position += right * stride; }
    if (Input::IsKeyPressed(GLFW_KEY_A)) { position -= right * stride; }
    if (Input::IsKeyPressed(GLFW_KEY_Z) && fovAngle <= 100.0f) { fovAngle += 1.0f; }
    if (Input::IsKeyPressed(GLFW_KEY_Y) && fovAngle >= 20.0f) { fovAngle -= 1.0f; }
}


void Camera::HandleMouseInput() {
    const Position mouseDelta = Input::GetMouseDelta();
    yawAngle += static_cast<GLfloat>(mouseDelta.x) * turnSpeed;
    pitchAngle += static_cast<GLfloat>(mouseDelta.y) * turnSpeed;
    if (pitchAngle > +89.0f) { pitchAngle = +89.0f; }
    if (pitchAngle < -89.0f) { pitchAngle = -89.0f; }
}


void Camera::AdjustOrientation() {
    front.x = cos(radians(pitchAngle)) * cos(radians(yawAngle));
    front.y = sin(radians(pitchAngle));
    front.z = cos(radians(pitchAngle)) * sin(radians(yawAngle));
    front = normalize(front);
    right = normalize(cross(front, worldUp));
    up = normalize(cross(right, front));
}
