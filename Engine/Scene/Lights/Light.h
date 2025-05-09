#pragma once
#include <GLM/glm.hpp>
class Mesh;


/// Holds color and intensity data for a lighting component.
typedef struct LightAttribute {
    glm::vec3 color;  ///< RGB color of the lighting component.
    float intensity;  ///< Intensity multiplier (0.0 to 1.0).
} LightComponent;


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
    void SetColor(const glm::vec3& color);

    /// Sets the intensity for all lighting components.
    /// @param intensity Intensity value between 0.0 and 1.0.
    void SetIntensity(float intensity);

    /// Sets color and intensity of the ambient component.
    /// @param color The RGB color to apply.
    /// @param intensity Intensity value between 0.0 and 1.0.
    void SetAmbient(const glm::vec3& color, float intensity);

    /// Sets color and intensity of the diffuse component.
    /// @param color The RGB color to apply.
    /// @param intensity Intensity value between 0.0 and 1.0.
    virtual void SetDiffuse(const glm::vec3& color, float intensity);

    /// Sets color and intensity of the specular component.
    /// @param color The RGB color to apply.
    /// @param intensity Intensity value between 0.0 and 1.0.
    virtual void SetSpecular(const glm::vec3& color, float intensity);

    /// Gets the type of the light.
    /// @return Light type enum.
    [[nodiscard]] Type GetType() const;

    /// Gets the ambient component of the light.
    /// @return Structure containing color and intensity.
    [[nodiscard]] LightComponent GetAmbient() const;

    /// Gets the diffuse component of the light.
    /// @return Structure containing color and intensity.
    [[nodiscard]] LightComponent GetDiffuse() const;

    /// Gets the specular component of the light.
    /// @return Structure containing color and intensity.
    [[nodiscard]] LightComponent GetSpecular() const;

    /// Gets the direction of the light.
    /// @return The direction vector (zero vector if not overridden).
    [[nodiscard]] virtual glm::vec3 GetDirection() const { return glm::vec3(0.0f); }

    /// Gets the position of the light.
    /// @return The position vector (zero vector if not overridden).
    [[nodiscard]] virtual glm::vec3 GetPosition() const { return glm::vec3(0.0f); }

    /// Virtual destructor for cleanup.
    virtual ~Light() = default;

protected:
    Type type; ///< The type of the light.
    LightComponent ambient;  ///< Ambient lighting component.
    LightComponent diffuse;  ///< Diffuse lighting component.
    LightComponent specular; ///< Specular lighting component.
};
