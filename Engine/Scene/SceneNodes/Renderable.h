#pragma once
#include "SceneNode.h"
#include "../../Graphics/Texturing/Material.h"
class Mesh; class ShaderProgram;
using namespace std;


class Renderable final : public SceneNode {
public:
    explicit Renderable(const string&, Mesh*);
    void SetMaterial(const Material&);
    void SetTexture(const string&);
    void Render(ShaderProgram&, const Transform& = Transform()) const;

private:
    Mesh* mesh;
    Material material;
};
