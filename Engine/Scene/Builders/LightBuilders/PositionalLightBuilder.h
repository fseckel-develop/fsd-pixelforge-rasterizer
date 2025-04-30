#pragma once
#include "LightBuilder.h"
#include "../../Lights/PositionalLight.h"


class PositionalLightBuilder final : public LightBuilder<PositionalLight, PositionalLightBuilder> {
public:
    PositionalLightBuilder() = default;

    auto& withPosition(const vec3& position) {
        light->SetPosition(position);
        return *this;
    }

    auto& withAttenuation(const Attenuation& attenuation) {
        light->SetAttenuation(attenuation);
        return *this;
    }

    auto& withRange(const LightRange range) {
        light->SetRange(range);
        return *this;
    }
};


inline auto PositionalLight_() {
    return PositionalLightBuilder();
}
