#pragma once
#include "LightBuilder.h"
#include "DirectionalLight.h"


class DirectionalLightBuilder : public LightBuilder<DirectionalLight, DirectionalLightBuilder> {
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
