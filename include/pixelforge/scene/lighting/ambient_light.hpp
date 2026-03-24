#pragma once
#include <pixelforge/scene/lighting/light.hpp>
#include <GLM/glm.hpp>


namespace pixelforge::scene::lighting {

    /// Extension of the Light class that only provides ambient illumination.
    class AmbientLight final : public Light {
    public:
        /// Default constructor.
        AmbientLight();

        /// Constructs an ambient light with a specified color and intensity.
        /// @param color The RGB color of the ambient component.
        /// @param intensity The intensity of the ambient component.
        AmbientLight(const glm::vec3& color, float intensity);

        /// Override with no effect since ambient light has no diffuse component.
        void setDiffuse(const glm::vec3& /*color*/, float /*intensity*/) override {}

        /// Override with no effect since ambient light has no specular component.
        void setSpecular(const glm::vec3& /*color*/, float /*intensity*/) override {}
    };

} // namespace pixelforge::scene::lighting
