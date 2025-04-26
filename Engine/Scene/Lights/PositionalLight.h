#pragma once
#include "Light.h"
#include <GLM/glm.hpp>
using namespace glm;


typedef struct Attenuation {
    float constant;
    float linear;
    float quadratic;
} Attenuation;


typedef enum LightRange {
    R3250, R600, R325, R200, R160, R100, R65, R50, R32, R20, R13, R7
} LightRange;


class PositionalLight : public Light {
public:
    PositionalLight(const vec3&, const Attenuation&);
    explicit PositionalLight(const vec3&, LightRange = R100);
    void SetPosition(const vec3&);
    void SetAttenuation(const Attenuation&);
    void SetAttenuationForRange(LightRange);
    [[nodiscard]] vec3 GetPosition() const override;
    [[nodiscard]] Attenuation GetAttenuation() const;

protected:
    vec3 position;
    Attenuation attenuation;
    static Attenuation GetAttenuationForRange(LightRange);
};
