#pragma once
#include <GLM/glm.hpp>
class ShaderProgram; class Mesh;
using namespace glm;


typedef struct LightAttribute {
    vec3 color;
    float intensity;
} LightAttribute;


class Light {
public:
    enum Type { AMBIENT = 0, DIRECTIONAL = 1, POSITIONAL = 2, SPOT = 3 };
    Light();
    void SetColor(const vec3&);
    void SetIntensity(float);
    void SetAmbient(const vec3&, float);
    virtual void SetDiffuse(const vec3&, float);
    virtual void SetSpecular(const vec3&, float);
    [[nodiscard]] Type GetType() const;
    [[nodiscard]] LightAttribute GetAmbient() const;
    [[nodiscard]] LightAttribute GetDiffuse() const;
    [[nodiscard]] LightAttribute GetSpecular() const;
    [[nodiscard]] virtual vec3 GetDirection() const { return vec3(0.0f); }
    [[nodiscard]] virtual vec3 GetPosition() const { return vec3(0.0f); }
    virtual ~Light() = default;

protected:
    Type type;
    LightAttribute ambient;
    LightAttribute diffuse;
    LightAttribute specular;
};
