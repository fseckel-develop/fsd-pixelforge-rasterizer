#pragma once
#include "LightBuilder.h"
#include "../../Lights/DirectionalLight.h"


class DirectionalLightBuilder final : public LightBuilder<DirectionalLight, DirectionalLightBuilder> {
public:
    DirectionalLightBuilder() = default;

    auto& withDirection(const vec3& direction) {
        light->SetDirection(direction);
        return *this;
    }
};


inline auto DirectionalLight_() {
    return DirectionalLightBuilder();
}
