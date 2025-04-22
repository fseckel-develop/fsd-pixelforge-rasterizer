#pragma once
#include "SceneNode.h"
class Mesh; class Material;
using namespace std;


class RenderUnit final : public SceneNode {
public:
    explicit RenderUnit(const string&, Mesh*);
    void SetMaterial(Material*);
    void SetTexture(const string&);
    [[nodiscard]] shared_ptr<Mesh>& GetMesh();
    [[nodiscard]] shared_ptr<Material>& GetMaterial();

private:
    shared_ptr<Mesh> mesh;
    shared_ptr<Material> material;
};
