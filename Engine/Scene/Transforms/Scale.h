#pragma once
#include <GLM/glm.hpp>
#include "Transform.h"
using namespace glm;


class Scale final {
public:
    Scale() = default;
    explicit Scale(float);
    explicit Scale(const vec3&);
    Scale(float, float, float);
    operator Transform() const { return scale; } // NOLINT

private:
    Transform scale;
};
