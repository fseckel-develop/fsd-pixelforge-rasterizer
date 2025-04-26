#pragma once
#include "PositionalLight.h"
#include <GLM/glm.hpp>
using namespace glm;


class SpotLight final : public PositionalLight {
public:
    SpotLight(const vec3&, const vec3&, const Attenuation&);
    SpotLight(const vec3&, const vec3&, LightRange = R3250);
    void SetDirection(const vec3&);
    void SetCutoffAngles(float, float);
    [[nodiscard]] vec3 GetDirection() const override;
    [[nodiscard]] float GetInnerCutoff() const;
    [[nodiscard]] float GetOuterCutoff() const;

private:
    vec3 direction;
    float innerCutoff;
    float outerCutoff;
};
