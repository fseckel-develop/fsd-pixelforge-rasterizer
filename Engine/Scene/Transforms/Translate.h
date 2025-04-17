#pragma once
#include "Transform.h"
#include <GLM/glm.hpp>
using namespace glm;


class Translate final : public Transform {
public:
    explicit Translate(const vec3&);
    Translate(float, float, float);
};
