#pragma once
#include "Transform.h"
#include <GLM/glm.hpp>
using namespace glm;


class Rotate final : public Transform {
public:
    explicit Rotate(const quat&);
    explicit Rotate(const vec3&);
    Rotate(float, float, float);
};
