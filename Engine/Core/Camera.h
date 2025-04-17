#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
using namespace glm;


class Camera {
public:
    static void TurnOn(vec3);
    static void Update();
    static mat4 GetViewMatrix();
    static mat4 GetProjectionMatrix();
    static vec3 GetPosition();

private:
    static vec3 position, front, up, right, worldUp;
    static GLfloat yawAngle, pitchAngle, fovAngle, movementSpeed, turnSpeed;
    static void HandleKeyInput();
    static void HandleMouseInput();
    static void AdjustOrientation();
};
