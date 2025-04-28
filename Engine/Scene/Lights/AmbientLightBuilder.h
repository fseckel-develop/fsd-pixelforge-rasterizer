#pragma once
#include "LightBuilder.h"
#include "AmbientLight.h"


class AmbientLightBuilder : public LightBuilder<AmbientLight, AmbientLightBuilder> {
public:
    AmbientLightBuilder() = default;

    auto& withDiffuse(const vec3& color, float intensity) = delete;
    auto& withSpecular(const vec3& color, float intensity) = delete;
};


inline auto AmbientLight_() {
    return AmbientLightBuilder();
}
