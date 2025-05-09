#include "Rotating.h"
#include "../Transforms/Transform.h"
#include "../../Utilities.h"
using namespace glm;


Rotating::Rotating(const Mode mode):
    Animation(mode),
    rotationAxis({0.0f, 1.0f, 0.0f}),
    totalAngle(radians(360.0f)) {
}


Rotating::Rotating(const Mode mode, const float duration, const vec3& axis, const float angle):
    Animation(mode, duration),
    rotationAxis() {
    this->rotationAxis = Utilities::ValidateDirection(axis, "Rotation::Rotation");
    this->totalAngle = radians(angle);
}


void Rotating::SetRotationAxis(const vec3& axis) {
    this->rotationAxis = Utilities::ValidateDirection(axis, "Rotation::SetRotationAxis");
}


void Rotating::SetTotalAngle(const float angle) {
    this->totalAngle = radians(angle);
}


const vec3& Rotating::GetRotationAxis() const {
    return rotationAxis;
}


float Rotating::GetTotalAngle() const {
    return totalAngle;
}


Transform Rotating::GetOffset() {
    Transform offset;
    const float angle = totalAngle * GetProgress();
    const mat4 rotationMatrix = rotate(mat4(1.0f), angle, rotationAxis);
    offset.SetRotation(quat_cast(rotationMatrix));
    return offset;
}
