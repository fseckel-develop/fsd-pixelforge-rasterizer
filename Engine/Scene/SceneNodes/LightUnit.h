#pragma once
#include "TransformNode.h"
#include "../Lights/Light.h"


class LightUnit final : public TransformNode {
public:
    explicit LightUnit(const string&);
    LightUnit(const string&, const shared_ptr<Light>&);
    void SetLight(const shared_ptr<Light>&);
    void SetMesh(const shared_ptr<Mesh>&);
    [[nodiscard]] const shared_ptr<Light>& GetLight() const;
    [[nodiscard]] vec3 GetCurrentPosition() const;
    [[nodiscard]] vec3 GetCurrentDirection() const;
    [[nodiscard]] mat4 GetModelMatrix() const override;
    [[nodiscard]] shared_ptr<Mesh>& GetMesh();
    [[nodiscard]] bool ToBeRendered() const;

private:
    shared_ptr<Light> light;
    shared_ptr<Mesh> mesh = nullptr;
    bool toBeRendered = false;
    void SetDefaultMesh(Light::Type);
};
