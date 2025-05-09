#pragma once
#include "LightBuilder.h"
#include "../../Lights/SpotLight.h"


/// Extension of the LightBuilder for building
/// spotlights using a fluent interface.
class SpotLightBuilder final : public LightBuilder<SpotLight, SpotLightBuilder> {
public:
    /// Starts the building process for a spotlight.
    SpotLightBuilder() = default;

    /// Sets the position of the spotlight being built.
    /// @param position The position vector for the light.
    /// @return Reference to this builder for method chaining.
    auto& withPosition(const glm::vec3& position) {
        light->SetPosition(position);
        return *this;
    }

    /// Sets the direction of the spotlight being built.
    /// @param direction The direction vector for the light.
    /// @return Reference to this builder for method chaining.
    auto& withDirection(const glm::vec3& direction) {
        light->SetDirection(direction);
        return *this;
    }

    /// Sets the attenuation directly for the spotlight being built.
    /// @param attenuation The Attenuation factors (constant, linear, quadratic).
    /// @return Reference to this builder for method chaining.
    auto& withAttenuation(const Attenuation& attenuation) {
        light->SetAttenuation(attenuation);
        return *this;
    }

    /// Sets the attenuation based on range for the spotlight being built.
    /// @param range The range of the light.
    /// @return Reference to this builder for method chaining.
    auto& withRange(const Range range) {
        light->SetRange(range);
        return *this;
    }

    /// Sets the cutoff angles for the spotlight being built.
    /// @param innerCutoff The inner cutoff angle in degrees.
    /// @param outerCutoff The outer cutoff angle in degrees.
    /// @return Reference to this builder for method chaining.
    auto& withCutoffAngles(const float innerCutoff, const float outerCutoff) {
        light->SetCutoffAngles(innerCutoff, outerCutoff);
        return *this;
    }
};


/// Alias function to begin building a spotlight.
/// @return Spotlight builder for fluent configuration.
inline auto SpotLight_() {
    return SpotLightBuilder();
}
