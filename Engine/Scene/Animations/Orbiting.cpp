#include "Orbiting.h"
#include "../Transforms/Transform.h"
#include <GLM/gtc/quaternion.hpp>


Orbiting::Orbiting(const float radius, const vec3 axis, const float angle, const float duration, const Mode mode):
    Rotation(axis, angle, duration, mode) {
    this->radius = radius;
}


float Orbiting::GetRadius() const {
    return radius;
}


Transform Orbiting::GetOffset() {
    Transform offset;
    const float currentAngle = radians(totalAngle * GetProgress());
    auto radiant = vec3(1.0f, 0.0f, 0.0f);
    if (abs(dot(radiant, rotationAxis)) > 0.99f) {
        radiant = vec3(0.0f, 1.0f, 0.0f);
    }
    radiant = normalize(cross(rotationAxis, radiant)) * radius;
    const quat rotation = angleAxis(currentAngle, rotationAxis);
    const vec3 orbitalPosition = rotation * radiant;
    offset.translation = orbitalPosition;
    offset.rotation = rotation;
    return offset;
}
