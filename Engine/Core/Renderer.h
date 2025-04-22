#pragma once
#include <GLM/glm.hpp>
#include <unordered_map>
#include <string>
class Scene; class Mesh; class Light; class RenderUnit;
class Model; class ShaderProgram; class Material;
using namespace std; using namespace glm;


class Renderer {
public:
    enum RenderMode { FILLED, WIREFRAME };
    enum Lighting { NONE, MODEL_LIGHT, SCENE_LIGHT };
    static void Initialize();
    static void Render(const shared_ptr<Mesh>&, RenderMode = WIREFRAME);
    static void Render(const shared_ptr<Light>&, RenderMode = FILLED);
    static void Render(const shared_ptr<RenderUnit>&, Lighting = NONE, RenderMode = FILLED);
    static void Render(const shared_ptr<Model>&, RenderMode = FILLED);
    static void Render(const shared_ptr<Scene>&, RenderMode = FILLED);

private:
    static unordered_map<string, shared_ptr<ShaderProgram>> shaderPrograms;
    static shared_ptr<ShaderProgram> currentProgram;
    static void InitializeShaderProgram(const string&);
    static void PrepareFrameGL(RenderMode, vec4 = vec4(0.0f, 0.0f, 0.0f, 1.0f));
    static void SetShaderProgram(const string&);
    static void SetCameraUniforms();
    static void SetLightingUniforms(const vector<shared_ptr<Light>>&);
    static void SetMaterialUniforms(const shared_ptr<Material>&);
    static void DrawMesh(const shared_ptr<Mesh>&);
    static void DrawLight(const shared_ptr<Light>&);
    static void DrawRenderUnit(const shared_ptr<RenderUnit>&, Lighting);
    static void DrawModel(const shared_ptr<Model>&);
    static void DrawScene(const shared_ptr<Scene>&);
    static void Draw(const shared_ptr<Mesh>&);
};
