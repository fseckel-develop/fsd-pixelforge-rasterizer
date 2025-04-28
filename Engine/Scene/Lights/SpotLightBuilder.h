#pragma once
#include "LightBuilder.h"
#include "SpotLight.h"


class SpotLightBuilder : public LightBuilder<SpotLight, SpotLightBuilder> {
public:
    SpotLightBuilder() = default;

    auto& withPosition(const vec3& position) {
        light->SetPosition(position);
        return *this;
    }

    auto& withAttenuation(const Attenuation& attenuation) {
        light->SetAttenuation(attenuation);
        return *this;
    }

    auto& withDirection(const vec3& direction) {
        light->SetDirection(direction);
        return *this;
    }

    auto& withRange(const LightRange range) {
        light->SetRange(range);
        return *this;
    }

    auto& withCutoffAngles(const float firstAngle, const float secondAngle) {
        light->SetCutoffAngles(firstAngle, secondAngle);
        return *this;
    }
};


inline auto SpotLight_() {
    return SpotLightBuilder();
}
