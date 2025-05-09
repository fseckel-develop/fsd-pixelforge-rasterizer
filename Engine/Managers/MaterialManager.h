#pragma once
#include "InstanceManager.h"
#include "../Graphics/Texturing/Material.h"


/// Extension of the InstanceManager class for managing
/// material instances and ensuring their uniqueness.
class MaterialManager final : public InstanceManager<Material, MaterialManager> {
public:
    /// Computes the hash for a material instance.
    /// @param material The material instance to hash.
    /// @return Hash value for the material.
    static size_t Hash(const Material& material) {
        size_t seed = 0;
        CombineHashes(seed, std::hash<std::shared_ptr<Texture>>{}(material.GetDiffuseMap()));
        CombineHashes(seed, std::hash<std::shared_ptr<Texture>>{}(material.GetSpecularMap()));
        CombineHashes(seed, std::hash<glm::vec4>{}(material.GetAmbient()));
        CombineHashes(seed, std::hash<glm::vec4>{}(material.GetDiffuse()));
        CombineHashes(seed, std::hash<glm::vec4>{}(material.GetSpecular()));
        CombineHashes(seed, std::hash<float>{}(material.GetShininess()));
        return seed;
    }

    /// Compares two materials for equality.
    /// @param left The first material to compare.
    /// @param right The second material to compare.
    /// @return True if the materials are equal, false otherwise.
    static bool Equals(const Material& left, const Material& right) {
        return left.GetDiffuseMap() == right.GetDiffuseMap() &&
            left.GetSpecularMap() == right.GetSpecularMap() &&
            left.GetAmbient() == right.GetAmbient() &&
            left.GetDiffuse() == right.GetDiffuse() &&
            left.GetSpecular() == right.GetSpecular() &&
            left.GetShininess() == right.GetShininess();
    }
};
