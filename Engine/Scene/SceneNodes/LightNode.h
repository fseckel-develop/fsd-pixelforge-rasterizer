#pragma once
#include "TransformNode.h"
#include "../Lights/Light.h"


class LightNode final : public TransformNode {
public:
    LightNode(const string&, const shared_ptr<Light>&);
    void SetMesh(const shared_ptr<Mesh>&);
    [[nodiscard]] const shared_ptr<Light>& GetLight() const;
    [[nodiscard]] vec3 GetCurrentPosition() const;
    [[nodiscard]] vec3 GetCurrentDirection() const;
    [[nodiscard]] shared_ptr<Mesh>& GetMesh();
    [[nodiscard]] bool ToBeRendered() const;

private:
    shared_ptr<Light> light;
    shared_ptr<Mesh> mesh = nullptr;
    bool toBeRendered = false;
};
