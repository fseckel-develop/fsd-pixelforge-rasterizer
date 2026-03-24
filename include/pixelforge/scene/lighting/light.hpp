#pragma once
#include <GLM/glm.hpp>


namespace pixelforge::scene::lighting {

    /// Holds color and intensity data for a lighting component.
    struct LightComponent {
        glm::vec3 color;  ///< RGB color of the lighting component.
        float intensity;  ///< Intensity multiplier (0.0 to 1.0).
    };


    /// Abstract base class for all light types, providing common attributes and methods
    /// for color, intensity and component control (ambient, diffuse, specular).
    class Light {
    public:
        /// Specifies the type of light, with integer assignment for GLSL usage.
        enum Type { AMBIENT = 0, DIRECTIONAL = 1, POSITIONAL = 2, SPOT = 3 };

        /// Default constructor, with full intensity and white color.
        Light();

        /// Sets the RGB color for all lighting components.
        /// @param color The RGB color to apply.
        void setColor(const glm::vec3& color);

        /// Sets the intensity for all lighting components.
        /// @param intensity Intensity value between 0.0 and 1.0.
        void setIntensity(float intensity);

        /// Sets color and intensity of the ambient component.
        /// @param color The RGB color to apply.
        /// @param intensity Intensity value between 0.0 and 1.0.
        void setAmbient(const glm::vec3& color, float intensity);

        /// Sets color and intensity of the diffuse component.
        /// @param color The RGB color to apply.
        /// @param intensity Intensity value between 0.0 and 1.0.
        virtual void setDiffuse(const glm::vec3& color, float intensity);

        /// Sets color and intensity of the specular component.
        /// @param color The RGB color to apply.
        /// @param intensity Intensity value between 0.0 and 1.0.
        virtual void setSpecular(const glm::vec3& color, float intensity);

        /// Gets the type of the light.
        /// @return Light type enum.
        [[nodiscard]] Type getType() const;

        /// Gets the ambient component of the light.
        /// @return Structure containing color and intensity.
        [[nodiscard]] LightComponent getAmbient() const;

        /// Gets the diffuse component of the light.
        /// @return Structure containing color and intensity.
        [[nodiscard]] LightComponent getDiffuse() const;

        /// Gets the specular component of the light.
        /// @return Structure containing color and intensity.
        [[nodiscard]] LightComponent getSpecular() const;

        /// Gets the direction of the light.
        /// @return The direction vector (zero vector if not overridden).
        [[nodiscard]] virtual glm::vec3 getDirection() const { return glm::vec3(0.0f); }

        /// Gets the position of the light.
        /// @return The position vector (zero vector if not overridden).
        [[nodiscard]] virtual glm::vec3 getPosition() const { return glm::vec3(0.0f); }

        /// Virtual destructor for cleanup.
        virtual ~Light() = default;

    protected:
        Type type_; ///< The type of the light.
        LightComponent ambient_;  ///< Ambient lighting component.
        LightComponent diffuse_;  ///< Diffuse lighting component.
        LightComponent specular_; ///< Specular lighting component.
    };

} // namespace pixelforge::scene::lighting
