#include "Orbiting.h"
#include "../Transforms/Transform.h"
#include <GLM/gtc/quaternion.hpp>


Orbiting::Orbiting(const Mode mode):
    Rotating(mode),
    radius(0.0f) {
}


Orbiting::Orbiting(const Mode mode, const float duration, const float radius, const vec3& axis, const float angle):
    Rotating(mode, duration, axis, angle) {
    this->radius = radius;
}


void Orbiting::SetRadius(const float radius) {
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
    offset.SetTranslation(orbitalPosition);
    offset.SetRotation(rotation);
    return offset;
}
