#pragma once
#include <pixelforge/scene/lighting/light.hpp>
#include <GLM/glm.hpp>


namespace pixelforge::scene::lighting {

    /// Extension of the Light class with a specified direction but no position.
    class DirectionalLight final : public Light {
    public:
        /// Default constructor with direction set to (0, -1, 0).
        DirectionalLight();

        /// Constructs a directional light with a specified direction.
        /// @param direction The direction vector for the light.
        explicit DirectionalLight(const glm::vec3& direction);

        /// Sets the light's direction.
        /// @param direction The direction vector for the light.
        void setDirection(const glm::vec3& direction);

        /// Gets the current direction of the light.
        /// @return Normalized direction vector.
        [[nodiscard]] glm::vec3 getDirection() const override;

    private:
        glm::vec3 direction_; ///< Normalized direction vector of the light.
    };

} // namespace pixelforge::scene::lighting
