#pragma once
#include <pixelforge/managers/instance_manager.hpp>
#include <pixelforge/graphics/texturing/material.hpp>


namespace pixelforge::management {

    /// Extension of the InstanceManager class for managing
    /// material instances and ensuring their uniqueness.
    class MaterialManager final : public InstanceManager<graphics::Material, MaterialManager> {
    public:
        /// Computes the hash for a material instance.
        /// @param material The material instance to hash.
        /// @return Hash value for the material.
        static size_t hash(const graphics::Material& material) {
            size_t seed = 0;
            combineHashes(seed, std::hash<std::shared_ptr<graphics::Texture>>{}(material.getDiffuseMap()));
            combineHashes(seed, std::hash<std::shared_ptr<graphics::Texture>>{}(material.getSpecularMap()));
            combineHashes(seed, std::hash<glm::vec4>{}(material.getAmbient()));
            combineHashes(seed, std::hash<glm::vec4>{}(material.getDiffuse()));
            combineHashes(seed, std::hash<glm::vec4>{}(material.getSpecular()));
            combineHashes(seed, std::hash<float>{}(material.getShininess()));
            return seed;
        }

        /// Compares two materials for equality.
        /// @param left The first material to compare.
        /// @param right The second material to compare.
        /// @return True if the materials are equal, false otherwise.
        static bool equals(const graphics::Material& left, const graphics::Material& right) {
            return left.getDiffuseMap() == right.getDiffuseMap() &&
                left.getSpecularMap() == right.getSpecularMap() &&
                left.getAmbient() == right.getAmbient() &&
                left.getDiffuse() == right.getDiffuse() &&
                left.getSpecular() == right.getSpecular() &&
                left.getShininess() == right.getShininess();
        }
    };

} // namespace pixelforge::management
