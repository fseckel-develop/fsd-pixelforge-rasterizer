#pragma once
#include "TransformNode.h"
class Mesh; class Material;
using namespace std;


class RenderUnit final : public TransformNode {
public:
    explicit RenderUnit(const string&, const shared_ptr<Mesh>&);
    void SetMaterial(const shared_ptr<Material>&);
    void SetTexture(const string&);
    [[nodiscard]] shared_ptr<Mesh>& GetMesh();
    [[nodiscard]] shared_ptr<Material>& GetMaterial();

private:
    shared_ptr<Mesh> mesh;
    shared_ptr<Material> material;
};
