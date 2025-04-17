#pragma once
#include "Texture.h"
#include <GLM/glm.hpp>
using namespace std;
using namespace glm;


class Material {
public:
    Material();
    explicit Material(const string&);
    Material(Texture*, Texture*, vec4, vec4, vec4, float);
    void SetDiffuseMap(Texture*);
    void SetSpecularMap(Texture*);
    void SetAmbientColor(vec4);
    void SetDiffuseColor(vec4);
    void SetSpecularColor(vec4);
    void SetShininess(float);
    [[nodiscard]] Texture* GetDiffuseMap() const;
    [[nodiscard]] Texture* GetSpecularMap() const;
    [[nodiscard]] vec4 GetAmbient() const;
    [[nodiscard]] vec4 GetDiffuse() const;
    [[nodiscard]] vec4 GetSpecular() const;
    [[nodiscard]] float GetShininess() const;
    void UnbindTextures() const;

protected:
    Texture* diffuseMap;
    Texture* specularMap;
    vec4 ambient = vec4(0.0f);
    vec4 diffuse = vec4(0.0f);
    vec4 specular = vec4(0.0f);
    float shininess = 0.0f;
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
