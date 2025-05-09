#pragma once
#include "Light.h"
#include <GLM/glm.hpp>


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
    void SetDiffuse(const glm::vec3& /*color*/, float /*intensity*/) override {}

    /// Override with no effect since ambient light has no specular component.
    void SetSpecular(const glm::vec3& /*color*/, float /*intensity*/) override {}
};
