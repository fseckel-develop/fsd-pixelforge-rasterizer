#pragma once
#include "Transform.h"
#include <GLM/glm.hpp>
using namespace glm;


class Translation final {
public:
    Translation() = default;
    explicit Translation(const vec3&);
    Translation(float, float, float);
    operator Transform() const { return translation; } // NOLINT

private:
    Transform translation;
};
