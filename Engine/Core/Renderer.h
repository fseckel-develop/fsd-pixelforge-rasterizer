#pragma once
#include "../Scene/Transforms/Transform.h"
#include <unordered_map>
#include <string>
class Scene; class ShaderProgram; class Light;
class Material; class Mesh; class Model;
using namespace std;


class Renderer {
public:
    Renderer();
    void Render(const Scene&) const;
    static void Render(const shared_ptr<Mesh>&);

private:
    unordered_map<string, ShaderProgram*> programs;
    void InitializeShaderProgram(const string&);
    static void SetLighting(ShaderProgram*, const vector<shared_ptr<Light>>&);
    static void SetMaterial(ShaderProgram*, const shared_ptr<Material>&);
    void RenderLights(const vector<shared_ptr<Light>>&) const;
    void RenderObjects(const shared_ptr<Model>&, const Transform& = Transform()) const;
};
