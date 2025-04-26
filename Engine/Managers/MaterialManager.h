#pragma once
#include "InstanceManager.h"
#include "../Graphics/Texturing/Material.h"
using namespace std;


class MaterialManager final : public InstanceManager<Material, MaterialManager> {
public:
    static size_t Hash(const Material& material) {
        size_t seed = 0;
        CombineHashes(seed, hash<shared_ptr<Texture>>{}(material.GetDiffuseMap()));
        CombineHashes(seed, hash<shared_ptr<Texture>>{}(material.GetSpecularMap()));
        CombineHashes(seed, hash<vec4>{}(material.GetAmbient()));
        CombineHashes(seed, hash<vec4>{}(material.GetDiffuse()));
        CombineHashes(seed, hash<vec4>{}(material.GetSpecular()));
        CombineHashes(seed, hash<float>{}(material.GetShininess()));
        return seed;
    }

    static bool Equals(const Material& left, const Material& right) {
        return left.GetDiffuseMap() == right.GetDiffuseMap() &&
            left.GetSpecularMap() == right.GetSpecularMap() &&
            left.GetAmbient() == right.GetAmbient() &&
            left.GetDiffuse() == right.GetDiffuse() &&
            left.GetSpecular() == right.GetSpecular() &&
            left.GetShininess() == right.GetShininess();
    }
};
