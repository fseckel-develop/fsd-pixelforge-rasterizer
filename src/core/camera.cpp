#include <pixelforge/core/camera.hpp>
#include <pixelforge/core/window.hpp>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include "input.hpp"


namespace pixelforge::core {

    using glm::vec3;
    using glm::mat4;
    using glm::perspective;
    using glm::radians;


    vec3 Camera::position_ = vec3(0.0f, 0.0f, 0.0f);
    vec3 Camera::front_ = vec3(0.0f, 0.0f, -1.0f);
    vec3 Camera::up_ = vec3(0.0f);
    vec3 Camera::right_ = vec3(0.0f);
    vec3 Camera::worldUp_ = vec3(0.0f, 1.0f, 0.0f);
    GLfloat Camera::yawAngle_ = -90.0f;
    GLfloat Camera::pitchAngle_ = 0.0f;
    GLfloat Camera::fovAngle_ = 45.0f;
    GLfloat Camera::movementSpeed_ = 5.0f;
    GLfloat Camera::turnSpeed_ = 0.1f;


    void Camera::turnOn(const vec3 position) {
        position_ = position;
        adjustOrientation();
    }


    void Camera::update() {
        handleKeyInput();
        handleMouseInput();
        adjustOrientation();
    }


    mat4 Camera::getViewMatrix() {
        return lookAt(position_, position_ + front_, up_);
    }


    mat4 Camera::getProjectionMatrix() {
        return perspective(radians(fovAngle_), Window::getAspectRatio(), 0.1f, 1000.0f);
    }


    vec3 Camera::getPosition() {
        return position_;
    }


    void Camera::handleKeyInput() {
        const GLfloat stride = movementSpeed_ * Input::getTimeDelta();
        if (Input::isKeyPressed(GLFW_KEY_W)) { position_ += front_ * stride; }
        if (Input::isKeyPressed(GLFW_KEY_S)) { position_ -= front_ * stride; }
        if (Input::isKeyPressed(GLFW_KEY_D)) { position_ += right_ * stride; }
        if (Input::isKeyPressed(GLFW_KEY_A)) { position_ -= right_ * stride; }
        if (Input::isKeyPressed(GLFW_KEY_Z) && fovAngle_ <= 100.0f) { fovAngle_ += 1.0f; }
        if (Input::isKeyPressed(GLFW_KEY_Y) && fovAngle_ >= 20.0f) { fovAngle_ -= 1.0f; }
    }


    void Camera::handleMouseInput() {
        const Position mouseDelta = Input::getMouseDelta();
        yawAngle_ += static_cast<GLfloat>(mouseDelta.x) * turnSpeed_;
        pitchAngle_ += static_cast<GLfloat>(mouseDelta.y) * turnSpeed_;
        if (pitchAngle_ > +89.0f) { pitchAngle_ = +89.0f; }
        if (pitchAngle_ < -89.0f) { pitchAngle_ = -89.0f; }
    }


    void Camera::adjustOrientation() {
        front_.x = cos(radians(pitchAngle_)) * cos(radians(yawAngle_));
        front_.y = sin(radians(pitchAngle_));
        front_.z = cos(radians(pitchAngle_)) * sin(radians(yawAngle_));
        front_ = normalize(front_);
        right_ = normalize(cross(front_, worldUp_));
        up_ = normalize(cross(right_, front_));
    }

} // namespace pixelforge::core
