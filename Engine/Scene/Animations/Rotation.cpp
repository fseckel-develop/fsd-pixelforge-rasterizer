#include "Rotation.h"
#include "../Transforms/Transform.h"
#include <GLM/gtc/quaternion.hpp>


Rotation::Rotation(const vec3 axis, const float angle, const float duration, const Mode mode):
    Animation(duration, mode) {
    this->totalAngle = angle;
    if (length(axis) != 0.0f) {
        this->rotationAxis = normalize(axis);
    }
}


float Rotation::GetTotalAngle() const {
    return totalAngle;
}


const vec3& Rotation::GetRotationAxis() const {
    return rotationAxis;
}


Transform Rotation::GetOffset() {
    Transform offset;
    const float angle = radians(totalAngle * GetProgress());
    const mat4 rotationMatrix = rotate(mat4(1.0f), angle, rotationAxis);
    offset.rotation = quat_cast(rotationMatrix);
    return offset;
}
