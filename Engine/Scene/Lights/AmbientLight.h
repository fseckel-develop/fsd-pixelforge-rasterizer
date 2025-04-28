#pragma once
#include "Light.h"
#include <GLM/glm.hpp>
using namespace glm;


class AmbientLight final : public Light {
public:
    AmbientLight();
    AmbientLight(const vec3&, float);
    void SetDiffuse(const vec3&, float) override {}
    void SetSpecular(const vec3&, float) override {}
};
