#pragma once
#include <pixelforge/graphics/texturing/texture.hpp>
#include <GLM/glm.hpp>


namespace pixelforge::graphics {

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
        void setDiffuseMap(const std::shared_ptr<Texture>& diffuseMap);

        /// Sets the specular texture map.
        /// @param specularMap Shared pointer to a texture.
        void setSpecularMap(const std::shared_ptr<Texture>& specularMap);

        /// Sets the ambient color component.
        /// @param ambientColor 4D color vector (RGBA).
        void setAmbientColor(const glm::vec4& ambientColor);

        /// Sets the diffuse color component.
        /// @param diffuseColor 4D color vector (RGBA).
        void setDiffuseColor(const glm::vec4& diffuseColor);

        /// Sets the specular color component.
        /// @param specularColor 4D color vector (RGBA).
        void setSpecularColor(const glm::vec4& specularColor);

        /// Sets the shininess factor used in specular lighting.
        /// @param shininess The shininess factor.
        void setShininess(float shininess);

        /// Gets the diffuse texture map of the material.
        /// @return Shared pointer to the bound diffuse map.
        [[nodiscard]] const std::shared_ptr<Texture>& getDiffuseMap() const;

        /// Gets the specular texture map of the material.
        /// @return Shared pointer to the bound specular map.
        [[nodiscard]] const std::shared_ptr<Texture>& getSpecularMap() const;

        /// Gets the ambient color of the material.
        /// @return Reference to the 4D color vector (RGBA).
        [[nodiscard]] const glm::vec4& getAmbient() const;

        /// Gets the diffuse color of the material.
        /// @return Reference to the 4D color vector (RGBA).
        [[nodiscard]] const glm::vec4& getDiffuse() const;

        /// Gets the specular color of the material.
        /// @return Reference to the 4D color vector (RGBA).
        [[nodiscard]] const glm::vec4& getSpecular() const;

        /// Gets the shininess factor of the material.
        /// @return The shininess factor.
        [[nodiscard]] float getShininess() const;

        /// Unbinds any of the bound textures, if present.
        void unbindTextures() const;

    protected:
        std::shared_ptr<Texture> diffuseMap_;  ///< Diffuse texture map.
        std::shared_ptr<Texture> specularMap_; ///< Specular texture map.
        glm::vec4 ambient_;  ///< Ambient color (RGBA).
        glm::vec4 diffuse_;  ///< Diffuse color (RGBA).
        glm::vec4 specular_; ///< Specular color (RGBA).
        float shininess_;    ///< Shininess factor for specular highlights.
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

} // namespace pixelforge::graphics
