#pragma once
#include <pixelforge/managers/instance_manager.hpp>
#include <pixelforge/scene/lighting/ambient_light.hpp>
#include <pixelforge/scene/lighting/directional_light.hpp>
#include <pixelforge/scene/lighting/positional_light.hpp>
#include <pixelforge/scene/lighting/spot_light.hpp>


namespace pixelforge::management {

    /// Extension of the InstanceManager class for managing
    /// light instances and ensuring their uniqueness.
    class LightManager final: public InstanceManager<scene::lighting::Light, LightManager> {
    public:
        /// Computes the hash for a light instance.
        /// @param light The light instance to hash.
        /// @return Hash value for the light.
        static size_t hash(const scene::lighting::Light& light) {
            size_t seed = typeid(light).hash_code();
            combineHashes(seed, std::hash<int>{}(light.getType()));
            combineHashes(seed, std::hash<glm::vec3>{}(light.getAmbient().color));
            combineHashes(seed, std::hash<float>{}(light.getAmbient().intensity));
            combineHashes(seed, std::hash<glm::vec3>{}(light.getDiffuse().color));
            combineHashes(seed, std::hash<float>{}(light.getDiffuse().intensity));
            combineHashes(seed, std::hash<glm::vec3>{}(light.getSpecular().color));
            combineHashes(seed, std::hash<float>{}(light.getSpecular().intensity));
            if (auto* directionalLight = dynamic_cast<const scene::lighting::DirectionalLight*>(&light)) {
                combineHashes(seed, hashLight(*directionalLight));
            } else if (auto* positionalLight = dynamic_cast<const scene::lighting::PositionalLight*>(&light)) {
                combineHashes(seed, hashLight(*positionalLight));
            } else if (auto* spotLight = dynamic_cast<const scene::lighting::SpotLight*>(&light)) {
                combineHashes(seed, hashLight(*spotLight));
            }
            return seed;
        }

        /// Compares two lights instances for equality.
        /// @param left The first light to compare.
        /// @param right The second light to compare.
        /// @return True if the lights are equal, false otherwise.
        static bool equals(const scene::lighting::Light& left, const scene::lighting::Light& right) {
            if (typeid(left) != typeid(right)) return false;
            if (left.getType() != right.getType()) return false;
            if (left.getAmbient().color != right.getAmbient().color ||
                left.getAmbient().intensity != right.getAmbient().intensity ||
                left.getDiffuse().color != right.getDiffuse().color ||
                left.getDiffuse().intensity != right.getDiffuse().intensity ||
                left.getSpecular().color != right.getSpecular().color ||
                left.getSpecular().intensity != right.getSpecular().intensity) {
                return false;
                }
            if (auto* leftDirectional = dynamic_cast<const scene::lighting::DirectionalLight*>(&left)) {
                auto* rightDirectional = dynamic_cast<const scene::lighting::DirectionalLight*>(&right);
                if (!rightDirectional) return false;
                return leftDirectional->getDirection() == rightDirectional->getDirection();
            }
            if (auto* leftPositional = dynamic_cast<const scene::lighting::PositionalLight*>(&left)) {
                auto* rightPositional = dynamic_cast<const scene::lighting::PositionalLight*>(&right);
                if (!rightPositional) return false;
                return leftPositional->getPosition() == rightPositional->getPosition() &&
                    leftPositional->getAttenuation().constant == rightPositional->getAttenuation().constant &&
                    leftPositional->getAttenuation().linear == rightPositional->getAttenuation().linear &&
                    leftPositional->getAttenuation().quadratic == rightPositional->getAttenuation().quadratic;
            }
            if (auto* leftSpot = dynamic_cast<const scene::lighting::SpotLight*>(&left)) {
                auto* rightSpot = dynamic_cast<const scene::lighting::SpotLight*>(&right);
                if (!rightSpot) return false;
                return leftSpot->getPosition() == rightSpot->getPosition() &&
                    leftSpot->getDirection() == rightSpot->getDirection() &&
                    leftSpot->getAttenuation().constant == rightSpot->getAttenuation().constant &&
                    leftSpot->getAttenuation().linear == rightSpot->getAttenuation().linear &&
                    leftSpot->getAttenuation().quadratic == rightSpot->getAttenuation().quadratic;
            }
            return false;
        }

    private:
        /// Computes the hash for a directional light instance.
        /// @param light Directional light instance to hash.
        /// @return The computed hash value for the directional light instance.
        static size_t hashLight(const scene::lighting::DirectionalLight& light) {
            return std::hash<glm::vec3>{}(light.getDirection());
        }

        /// Computes the hash for a positional light instance.
        /// @param light Positional light instance to hash.
        /// @return The computed hash value for the positional light instance.
        static size_t hashLight(const scene::lighting::PositionalLight& light) {
            size_t seed = 0;
            combineHashes(seed, std::hash<glm::vec3>{}(light.getPosition()));
            combineHashes(seed, std::hash<float>{}(light.getAttenuation().constant));
            combineHashes(seed, std::hash<float>{}(light.getAttenuation().linear));
            combineHashes(seed, std::hash<float>{}(light.getAttenuation().quadratic));
            return seed;
        }

        /// Computes the hash for a spotlight instance.
        /// @param light Spotlight instance to hash.
        /// @return The computed hash value for the spotlight instance.
        static size_t hashLight(const scene::lighting::SpotLight& light) {
            size_t seed = 0;
            combineHashes(seed, std::hash<glm::vec3>{}(light.getPosition()));
            combineHashes(seed, std::hash<glm::vec3>{}(light.getDirection()));
            combineHashes(seed, std::hash<float>{}(light.getAttenuation().constant));
            combineHashes(seed, std::hash<float>{}(light.getAttenuation().linear));
            combineHashes(seed, std::hash<float>{}(light.getAttenuation().quadratic));
            return seed;
        }
    };

} // namespace pixelforge::management
