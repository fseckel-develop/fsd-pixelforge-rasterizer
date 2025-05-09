#pragma once
#include "LightBuilder.h"
#include "../../Lights/PositionalLight.h"


/// Extension of the LightBuilder class for building
/// positional lights using a fluent interface.
class PositionalLightBuilder final : public LightBuilder<PositionalLight, PositionalLightBuilder> {
public:
    /// Starts the building process for a positional light.
    PositionalLightBuilder() = default;

    /// Sets the position of the positional light being built.
    /// @param position The position vector for the light.
    /// @return Reference to this builder for method chaining.
    auto& withPosition(const glm::vec3& position) {
        light->SetPosition(position);
        return *this;
    }

    /// Sets the attenuation directly for the positional light being built.
    /// @param attenuation The attenuation factors (constant, linear, quadratic).
    /// @return Reference to this builder for method chaining.
    auto& withAttenuation(const Attenuation& attenuation) {
        light->SetAttenuation(attenuation);
        return *this;
    }

    /// Sets the attenuation based on range for the positional light being built.
    /// @param range The range of the light.
    /// @return Reference to this builder for method chaining.
    auto& withRange(const Range range) {
        light->SetRange(range);
        return *this;
    }
};


/// Alias function to begin building a positional light.
/// @return Positional light builder for fluent configuration.
inline auto PositionalLight_() {
    return PositionalLightBuilder();
}
