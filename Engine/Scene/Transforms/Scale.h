#pragma once
#include "Transform.h"
#include <GLM/glm.hpp>
using namespace glm;


class Scale final : public Transform {
public:
    Scale() = default;
    explicit Scale(const vec3&);
    Scale(float, float, float);
};
