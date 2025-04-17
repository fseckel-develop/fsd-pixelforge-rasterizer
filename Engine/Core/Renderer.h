#pragma once
#include <unordered_map>
class Scene; class ShaderProgram; class Mesh;
class Light; class Renderable; class Material;
using namespace std;


class Renderer {
public:
    Renderer() = default;
    void Render(const Scene&) const;

private:
    unordered_map<string, ShaderProgram> programs;
    void DrawMesh(const Mesh&) const;
    void RenderLight(const Light&) const;
    void RenderRenderable(const Renderable&) const;
    void UploadLightsToShader(const vector<Light*>&, ShaderProgram&) const;
    void UploadMaterialToShader(const Material&, ShaderProgram&) const;
};
