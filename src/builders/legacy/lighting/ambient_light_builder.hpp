#pragma once
#include "builders/legacy/lighting/light_builder.hpp"
#include <pixelforge/scene/lighting/ambient_light.hpp>


namespace pixelforge::scene::lighting {

    /// Extension of the LightBuilder class for building
    /// ambient lights using a fluent interface.
    class AmbientLightBuilder final : public LightBuilder<AmbientLight, AmbientLightBuilder> {
    public:
        /// Starts the building process for an ambient light.
        AmbientLightBuilder() = default;

        /// Deleted. Ambient lights do not support a diffuse lighting component.
        auto& withDiffuse(const glm::vec3& color, float intensity) = delete;

        /// Deleted. Ambient lights do not support a specular lighting component.
        auto& withSpecular(const glm::vec3& color, float intensity) = delete;
    };


    /// Alias function to begin building an ambient light.
    /// @return Ambient light builder for fluent configuration.
    inline auto AmbientLight_() {
        return AmbientLightBuilder();
    }

} // namespace pixelforge::scene::lighting
