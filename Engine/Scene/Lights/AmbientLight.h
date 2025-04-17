#pragma once
#include "../SceneNodes/Light.h"
#include <GLM/glm.hpp>
using namespace glm;


class AmbientLight final : public Light {
public:
    explicit AmbientLight(const string&, const vec3&, float = 1.0f);
    void SetDiffuse(const vec3&, float) override {}
    void SetSpecular(const vec3&, float) override {}
};
