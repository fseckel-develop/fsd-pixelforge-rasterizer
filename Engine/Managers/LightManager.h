#pragma once
#include "InstanceManager.h"
#include "../Scene/Lights/AmbientLight.h"
#include "../Scene/Lights/DirectionalLight.h"
#include "../Scene/Lights/PositionalLight.h"
#include "../Scene/Lights/SpotLight.h"


class LightManager final: public InstanceManager<Light, LightManager> {
public:
    static size_t Hash(const Light& light) {
        size_t seed = typeid(light).hash_code();
        CombineHashes(seed, hash<int>{}(light.GetType()));
        CombineHashes(seed, hash<vec3>{}(light.GetAmbient().color));
        CombineHashes(seed, hash<float>{}(light.GetAmbient().intensity));
        CombineHashes(seed, hash<vec3>{}(light.GetDiffuse().color));
        CombineHashes(seed, hash<float>{}(light.GetDiffuse().intensity));
        CombineHashes(seed, hash<vec3>{}(light.GetSpecular().color));
        CombineHashes(seed, hash<float>{}(light.GetSpecular().intensity));
        if (auto* directionalLight = dynamic_cast<const DirectionalLight*>(&light)) {
            CombineHashes(seed, HashLight(*directionalLight));
        } else if (auto* positionalLight = dynamic_cast<const PositionalLight*>(&light)) {
            CombineHashes(seed, HashLight(*positionalLight));
        } else if (auto* spotLight = dynamic_cast<const SpotLight*>(&light)) {
            CombineHashes(seed, HashLight(*spotLight));
        }
        return seed;
    }

    static bool Equals(const Light& left, const Light& right) {
        if (typeid(left) != typeid(right)) return false;
        if (left.GetType() != right.GetType()) return false;
        if (left.GetAmbient().color != right.GetAmbient().color ||
            left.GetAmbient().intensity != right.GetAmbient().intensity ||
            left.GetDiffuse().color != right.GetDiffuse().color ||
            left.GetDiffuse().intensity != right.GetDiffuse().intensity ||
            left.GetSpecular().color != right.GetSpecular().color ||
            left.GetSpecular().intensity != right.GetSpecular().intensity) {
            return false;
        }
        if (auto* leftDirectional = dynamic_cast<const DirectionalLight*>(&left)) {
            auto* rightDirectional = dynamic_cast<const DirectionalLight*>(&right);
            if (!rightDirectional) return false;
            return leftDirectional->GetDirection() == rightDirectional->GetDirection();
        }
        if (auto* leftPositional = dynamic_cast<const PositionalLight*>(&left)) {
            auto* rightPositional = dynamic_cast<const PositionalLight*>(&right);
            if (!rightPositional) return false;
            return leftPositional->GetPosition() == rightPositional->GetPosition() &&
                leftPositional->GetAttenuation().constant == rightPositional->GetAttenuation().constant &&
                leftPositional->GetAttenuation().linear == rightPositional->GetAttenuation().linear &&
                leftPositional->GetAttenuation().quadratic == rightPositional->GetAttenuation().quadratic;
        }
        if (auto* leftSpot = dynamic_cast<const SpotLight*>(&left)) {
            auto* rightSpot = dynamic_cast<const SpotLight*>(&right);
            if (!rightSpot) return false;
            return leftSpot->GetPosition() == rightSpot->GetPosition() &&
                leftSpot->GetDirection() == rightSpot->GetDirection() &&
                leftSpot->GetAttenuation().constant == rightSpot->GetAttenuation().constant &&
                leftSpot->GetAttenuation().linear == rightSpot->GetAttenuation().linear &&
                leftSpot->GetAttenuation().quadratic == rightSpot->GetAttenuation().quadratic;
        }
        return false;
    }

private:
    static size_t HashLight(const DirectionalLight& light) {
        return hash<vec3>{}(light.GetDirection());
    }

    static size_t HashLight(const PositionalLight& light) {
        size_t seed = 0;
        CombineHashes(seed, hash<vec3>{}(light.GetPosition()));
        CombineHashes(seed, hash<float>{}(light.GetAttenuation().constant));
        CombineHashes(seed, hash<float>{}(light.GetAttenuation().linear));
        CombineHashes(seed, hash<float>{}(light.GetAttenuation().quadratic));
        return seed;
    }

    static size_t HashLight(const SpotLight& light) {
        size_t seed = 0;
        CombineHashes(seed, hash<vec3>{}(light.GetPosition()));
        CombineHashes(seed, hash<vec3>{}(light.GetDirection()));
        CombineHashes(seed, hash<float>{}(light.GetAttenuation().constant));
        CombineHashes(seed, hash<float>{}(light.GetAttenuation().linear));
        CombineHashes(seed, hash<float>{}(light.GetAttenuation().quadratic));
        return seed;
    }
};
