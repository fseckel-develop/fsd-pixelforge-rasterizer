#pragma once
#include "Texture.h"
#include <GLM/glm.hpp>


/// Represents a surface material used in rendering,
/// supporting texture maps and Phong shading.
class Material {
public:
    /// Default constructor initializing zero colors and no bound textures.
    Material();

    /// Constructs a material using a texture file path.
    /// @param path File path to the diffuse texture.
    explicit Material(const std::string& path);

    /// Sets the diffuse texture map.
    /// @param diffuseMap Shared pointer to a texture.
    void SetDiffuseMap(const std::shared_ptr<Texture>& diffuseMap);

    /// Sets the specular texture map.
    /// @param specularMap Shared pointer to a texture.
    void SetSpecularMap(const std::shared_ptr<Texture>& specularMap);

    /// Sets the ambient color component.
    /// @param ambientColor 4D color vector (RGBA).
    void SetAmbientColor(const glm::vec4& ambientColor);

    /// Sets the diffuse color component.
    /// @param diffuseColor 4D color vector (RGBA).
    void SetDiffuseColor(const glm::vec4& diffuseColor);

    /// Sets the specular color component.
    /// @param specularColor 4D color vector (RGBA).
    void SetSpecularColor(const glm::vec4& specularColor);

    /// Sets the shininess factor used in specular lighting.
    /// @param shininess The shininess factor.
    void SetShininess(float shininess);

    /// Gets the diffuse texture map of the material.
    /// @return Shared pointer to the bound diffuse map.
    [[nodiscard]] const std::shared_ptr<Texture>& GetDiffuseMap() const;

    /// Gets the specular texture map of the material.
    /// @return Shared pointer to the bound specular map.
    [[nodiscard]] const std::shared_ptr<Texture>& GetSpecularMap() const;

    /// Gets the ambient color of the material.
    /// @return Reference to the 4D color vector (RGBA).
    [[nodiscard]] const glm::vec4& GetAmbient() const;

    /// Gets the diffuse color of the material.
    /// @return Reference to the 4D color vector (RGBA).
    [[nodiscard]] const glm::vec4& GetDiffuse() const;

    /// Gets the specular color of the material.
    /// @return Reference to the 4D color vector (RGBA).
    [[nodiscard]] const glm::vec4& GetSpecular() const;

    /// Gets the shininess factor of the material.
    /// @return The shininess factor.
    [[nodiscard]] float GetShininess() const;

    /// Unbinds any of the bound textures, if present.
    void UnbindTextures() const;

protected:
    std::shared_ptr<Texture> diffuseMap;  ///< Diffuse texture map.
    std::shared_ptr<Texture> specularMap; ///< Specular texture map.
    glm::vec4 ambient;  ///< Ambient color (RGBA).
    glm::vec4 diffuse;  ///< Diffuse color (RGBA).
    glm::vec4 specular; ///< Specular color (RGBA).
    float shininess;    ///< Shininess factor for specular highlights.
};


class Gold final : public Material { public: Gold(); };
class Brass final : public Material { public: Brass(); };
class Chrome final : public Material { public: Chrome(); };
class Copper final : public Material { public: Copper(); };
class Silver final : public Material { public: Silver(); };
class Bronze final : public Material { public: Bronze(); };

class Ruby final : public Material { public: Ruby(); };
class Jade final : public Material { public: Jade(); };
class Pearl final : public Material { public: Pearl(); };
class Emerald final : public Material { public: Emerald(); };
class Obsidian final : public Material { public: Obsidian(); };
class Turquoise final : public Material { public: Turquoise(); };

class RedPlastic final : public Material { public: RedPlastic(); };
class CyanPlastic final : public Material { public: CyanPlastic(); };
class GreenPlastic final : public Material { public: GreenPlastic(); };
class YellowPlastic final : public Material { public: YellowPlastic(); };
class BlackPlastic final : public Material { public: BlackPlastic(); };
class WhitePlastic final : public Material { public: WhitePlastic(); };

class RedRubber final : public Material { public: RedRubber(); };
class CyanRubber final : public Material { public: CyanRubber(); };
class GreenRubber final : public Material { public: GreenRubber(); };
class YellowRubber final : public Material { public: YellowRubber(); };
class BlackRubber final : public Material { public: BlackRubber(); };
class WhiteRubber final : public Material { public: WhiteRubber(); };
