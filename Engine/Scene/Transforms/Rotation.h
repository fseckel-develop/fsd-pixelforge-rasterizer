#pragma once
#include "Transform.h"
#include <GLM/glm.hpp>
using namespace glm;


class Rotation final {
public:
    Rotation() = default;
    explicit Rotation(const quat&);
    explicit Rotation(const vec3&);
    Rotation(float, float, float);
    operator Transform() const { return rotation; } // NOLINT

private:
    Transform rotation;
};
