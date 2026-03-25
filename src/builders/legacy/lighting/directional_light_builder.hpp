#pragma once
#include "light_builder.hpp"
#include <pixelforge/scene/lighting/directional_light.hpp>


namespace pixelforge::scene::lighting {

    /// Extension of the LightBuilder class for building
    /// directional lights using a fluent interface.
    class DirectionalLightBuilder final : public LightBuilder<DirectionalLight, DirectionalLightBuilder> {
    public:
        /// Starts the building process for a directional light.
        DirectionalLightBuilder() = default;

        /// Sets the direction of the directional light being built.
        /// @param direction The direction vector for the light.
        /// @return Reference to the builder for chaining.
        auto& withDirection(const glm::vec3& direction) {
            light_->setDirection(direction);
            return *this;
        }
    };


    /// Alias function to begin building a directional light.
    /// @return Directional light builder for fluent configuration.
    inline auto DirectionalLight_() {
        return DirectionalLightBuilder();
    }

} // namespace pixelforge::scene::lighting
