#pragma once
#include "Light.h"
#include <GLM/glm.hpp>
using namespace glm;


class DirectionalLight final : public Light {
public:
    DirectionalLight();
    explicit DirectionalLight(const vec3&);
    void SetDirection(const vec3&);
    [[nodiscard]] vec3 GetDirection() const override;

private:
    vec3 direction;
};
