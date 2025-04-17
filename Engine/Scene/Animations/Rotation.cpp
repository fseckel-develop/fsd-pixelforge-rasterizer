#include "Rotation.h"
#include "../Transforms/Transform.h"
#include <GLM/gtc/quaternion.hpp>


Rotation::Rotation(const vec3 axis, const float angle, const float duration, const Mode mode):
    Animation(mode) {
    this->totalAngle = angle;
    if (length(axis) != 0.0f) {
        this->rotationAxis = normalize(axis);
    }
    this->duration = duration;
}


Transform Rotation::GetOffset(const float deltaTime) {
    Transform offset;
    if (IsPlaying()) {
        elapsedTime += deltaTime;
        const float angle = radians(totalAngle * GetProgress());
        const mat4 rotationMatrix = rotate(mat4(1.0f), angle, rotationAxis);
        offset.rotation = quat_cast(rotationMatrix);
    }
    return offset;
}
