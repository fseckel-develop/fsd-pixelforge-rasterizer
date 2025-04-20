#pragma once
#include "SceneNode.h"
#include <GLM/glm.hpp>
class ShaderProgram; class Mesh;
using namespace glm;


typedef enum LightType {
    AMBIENT = 0,
    DIRECTIONAL = 1,
    POSITIONAL = 2,
    SPOT = 3
} LightType;


typedef struct LightAttribute {
    vec3 color;
    float intensity;
} LightAttribute;


class Light : public SceneNode {
public:
    explicit Light(const string&, LightType);
    void SetAmbient(const vec3&, float);
    virtual void SetDiffuse(const vec3&, float);
    virtual void SetSpecular(const vec3&, float);
    void SetMesh(Mesh*);
    [[nodiscard]] LightType GetType() const;
    [[nodiscard]] LightAttribute GetAmbient() const;
    [[nodiscard]] LightAttribute GetDiffuse() const;
    [[nodiscard]] LightAttribute GetSpecular() const;
    [[nodiscard]] virtual vec3 GetCurrentPosition() const;
    [[nodiscard]] virtual vec3 GetCurrentDirection() const;
    [[nodiscard]] bool ToBeRendered() const;
    [[nodiscard]] shared_ptr<Mesh>& GetMesh();

protected:
    LightType type;
    LightAttribute ambient = LightAttribute(vec3(1.0f), 1.0f);
    LightAttribute diffuse = LightAttribute(vec3(1.0f), 1.0f);
    LightAttribute specular = LightAttribute(vec3(1.0f), 1.0f);
    shared_ptr<Mesh> mesh = nullptr;
    bool toBeRendered = false;
};
