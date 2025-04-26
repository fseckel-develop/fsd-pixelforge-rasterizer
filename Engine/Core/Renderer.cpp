#include "Renderer.h"
#include "Camera.h"
#include "Input.h"
#include "../Scene.h"
#include "../Utilities.h"
#include "../Geometry/Meshes/Mesh.h"
#include "../Graphics/Pipeline/VertexArray.h"
#include "../Graphics/Pipeline/ShaderProgram.h"
#include "../Graphics/Texturing/Material.h"


unordered_map<string, shared_ptr<ShaderProgram>> Renderer::shaderPrograms;
shared_ptr<ShaderProgram> Renderer::currentProgram = nullptr;
auto fallbackMaterial = make_shared<Material>(WhiteRubber());
vector fallbackLights = {
    make_shared<LightNode>("Ambient", shared<AmbientLight>(vec3(1.0f, 1.0f, 1.0f), 1.0f)),
    make_shared<LightNode>("Directional", shared<DirectionalLight>(vec3(1.0f, -2.0f, -2.0f)))
};


void Renderer::Initialize() {
    InitializeShaderProgram("White");
    InitializeShaderProgram("Emissive");
    InitializeShaderProgram("Lighting");
}


void Renderer::InitializeShaderProgram(const string& name) {
    const auto program = make_shared<ShaderProgram>();
    const string vertexShader = name + ".vert";
    const string fragmentShader = name + ".frag";
    program->AddShader(GL_VERTEX_SHADER, vertexShader.c_str());
    program->AddShader(GL_FRAGMENT_SHADER, fragmentShader.c_str());
    program->ActivateProgram();
    shaderPrograms[name] = program;
}


void Renderer::Render(const shared_ptr<Mesh>& mesh, const RenderMode mode) {
    PrepareFrameGL(mode);
    DrawMesh(mesh);
}


void Renderer::Render(const shared_ptr<LightNode>& light, const RenderMode mode) {
    light->UpdateSelfAndChildren(Input::GetDeltaTime());
    PrepareFrameGL(mode);
    DrawLight(light);
}


void Renderer::Render(const shared_ptr<RenderUnit>& renderUnit, const Lighting lighting, const RenderMode mode) {
    renderUnit->UpdateSelfAndChildren(Input::GetDeltaTime());
    PrepareFrameGL(mode);
    DrawRenderUnit(renderUnit, lighting);
}


void Renderer::Render(const shared_ptr<Model>& model, const RenderMode mode) {
    model->UpdateSelfAndChildren(Input::GetDeltaTime());
    PrepareFrameGL(mode);
    DrawModel(model);
}


void Renderer::Render(const shared_ptr<Scene>& scene, const RenderMode mode) {
    scene->UpdateSelfAndChildren(Input::GetDeltaTime());
    PrepareFrameGL(mode);
    DrawScene(scene);
}


void Renderer::PrepareFrameGL(const RenderMode mode, const vec4 clearColor) {
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, mode == WIREFRAME ? GL_LINE : GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}


void Renderer::SetShaderProgram(const string& name) {
    if (shaderPrograms.contains(name) && currentProgram != shaderPrograms.at(name)) {
        ShaderProgram::UnuseProgram();
        currentProgram = shaderPrograms.at(name);
    }
}


void Renderer::SetCameraUniforms() {
    currentProgram->SetUniform("viewMatrix", Camera::GetViewMatrix());
    currentProgram->SetUniform("projectionMatrix", Camera::GetProjectionMatrix());
    currentProgram->SetUniform("cameraPosition", Camera::GetPosition());
}


void Renderer::SetLightingUniforms(const vector<shared_ptr<LightNode>>& lightNodes) {
    if (currentProgram != shaderPrograms.at("Lighting")) SetShaderProgram("Lighting");
    currentProgram->SetUniform("lightCount", static_cast<int>(lightNodes.size()));
    for (size_t i = 0; i < lightNodes.size(); i++) {
        auto light = lightNodes.at(i)->GetLight();
        currentProgram->SetUniform(ArrayElement("lights", i, "type"), static_cast<int>(light->GetType()));
        currentProgram->SetUniform(ArrayElement("lights", i, "ambient.color"), light->GetAmbient().color);
        currentProgram->SetUniform(ArrayElement("lights", i, "ambient.intensity"), light->GetAmbient().intensity);
        currentProgram->SetUniform(ArrayElement("lights", i, "diffuse.color"), light->GetDiffuse().color);
        currentProgram->SetUniform(ArrayElement("lights", i, "diffuse.intensity"), light->GetDiffuse().intensity);
        currentProgram->SetUniform(ArrayElement("lights", i, "specular.color"), light->GetSpecular().color);
        currentProgram->SetUniform(ArrayElement("lights", i, "specular.intensity"), light->GetSpecular().intensity);
        switch (light->GetType()) {
            case AMBIENT: break;
            case DIRECTIONAL: {
                const auto directionalLight = dynamic_pointer_cast<DirectionalLight>(light);
                currentProgram->SetUniform(ArrayElement("lights", i, "direction"), lightNodes.at(i)->GetCurrentDirection());
                break;
            }
            case POSITIONAL: {
                const auto positionalLight = dynamic_pointer_cast<PositionalLight>(light);
                currentProgram->SetUniform(ArrayElement("lights", i, "position"), lightNodes.at(i)->GetCurrentPosition());
                currentProgram->SetUniform(ArrayElement("lights", i, "attenuation.constant"), positionalLight->GetAttenuation().constant);
                currentProgram->SetUniform(ArrayElement("lights", i, "attenuation.linear"), positionalLight->GetAttenuation().linear);
                currentProgram->SetUniform(ArrayElement("lights", i, "attenuation.quadratic"), positionalLight->GetAttenuation().quadratic);
                break;
            }
            case SPOT: {
                const auto spotLight = dynamic_pointer_cast<SpotLight>(light);
                currentProgram->SetUniform(ArrayElement("lights", i, "direction"), lightNodes.at(i)->GetCurrentDirection());
                currentProgram->SetUniform(ArrayElement("lights", i, "position"), lightNodes.at(i)->GetCurrentPosition());
                currentProgram->SetUniform(ArrayElement("lights", i, "attenuation.constant"), spotLight->GetAttenuation().constant);
                currentProgram->SetUniform(ArrayElement("lights", i, "attenuation.linear"), spotLight->GetAttenuation().linear);
                currentProgram->SetUniform(ArrayElement("lights", i, "attenuation.quadratic"), spotLight->GetAttenuation().quadratic);
                currentProgram->SetUniform(ArrayElement("lights", i, "innerCutoff"), spotLight->GetInnerCutoff());
                currentProgram->SetUniform(ArrayElement("lights", i, "outerCutoff"), spotLight->GetOuterCutoff());
                break;
            }
        }
    }
}


void Renderer::SetMaterialUniforms(const shared_ptr<Material>& material) {
    if (currentProgram != shaderPrograms.at("Lighting")) SetShaderProgram("Lighting");
    if (material->GetDiffuseMap()) {
        currentProgram->SetUniform("material.useDiffuseMap", true);
        currentProgram->SetUniform("material.diffuseMap", material->GetDiffuseMap()->BindTexture());
    } else {
        currentProgram->SetUniform("material.useDiffuseMap", false);
    }
    if (material->GetSpecularMap()) {
        currentProgram->SetUniform("material.useSpecularMap", true);
        currentProgram->SetUniform("material.specularMap", material->GetSpecularMap()->BindTexture());
    } else {
        currentProgram->SetUniform("material.useSpecularMap", false);
    }
    currentProgram->SetUniform("material.ambient", material->GetAmbient());
    currentProgram->SetUniform("material.diffuse", material->GetDiffuse());
    currentProgram->SetUniform("material.specular", material->GetSpecular());
    currentProgram->SetUniform("material.shininess", material->GetShininess());
}


void Renderer::DrawMesh(const shared_ptr<Mesh>& mesh) {
    SetShaderProgram("White");
    SetCameraUniforms();
    currentProgram->SetUniform("modelMatrix", mat4(1.0f));
    Draw(mesh);
}


void Renderer::DrawLight(const shared_ptr<LightNode>& lightNode) {
    if (lightNode->ToBeRendered()) {
        SetShaderProgram("Emissive");
        currentProgram->SetUniform("modelMatrix", lightNode->GetModelMatrix());
        currentProgram->SetUniform("viewMatrix", Camera::GetViewMatrix());
        currentProgram->SetUniform("projectionMatrix", Camera::GetProjectionMatrix());
        currentProgram->SetUniform("emissive.color", lightNode->GetLight()->GetAmbient().color);
        currentProgram->SetUniform("emissive.intensity", lightNode->GetLight()->GetAmbient().intensity);
        Draw(lightNode->GetMesh());
    }
}


void Renderer::DrawRenderUnit(const shared_ptr<RenderUnit>& renderUnit, const Lighting lighting) {
    SetShaderProgram("Lighting");
    SetCameraUniforms();
    currentProgram->SetUniform("modelMatrix", renderUnit->GetModelMatrix());
    if (lighting == NONE) SetLightingUniforms(fallbackLights);
    SetMaterialUniforms(renderUnit->GetMaterial() ? renderUnit->GetMaterial() : fallbackMaterial);
    Draw(renderUnit->GetMesh());
}


void Renderer::DrawModel(const shared_ptr<Model>& model) {
    for (const auto& light : model->GetLights()) {
        DrawLight(light);
    }
    const bool hasLights = !model->GetLights().empty();
    SetLightingUniforms(hasLights ? model->GetLights() : fallbackLights);
    for (const auto& renderUnit : model->GetRenderUnits()) {
        DrawRenderUnit(renderUnit, hasLights ? MODEL_LIGHT : NONE);
    }
}



void Renderer::DrawScene(const shared_ptr<Scene>& scene) {
    for (const auto& light : scene->GetAllLights()) {
        DrawLight(light);
    }
    const bool hasLights = !scene->GetAllLights().empty();
    SetLightingUniforms(hasLights ? scene->GetAllLights() : fallbackLights);
    for (const auto& model : scene->GetModels()) {
        for (const auto& renderUnit : model->GetRenderUnits()) {
            DrawRenderUnit(renderUnit, hasLights ? SCENE_LIGHT : NONE);
        }
    }
}


void Renderer::Draw(const shared_ptr<Mesh>& mesh) {
    currentProgram->UseProgram();
    mesh->GetVAO()->BindVAO();
    if (mesh->GetIndices().empty()) {
        glDrawArrays(GL_TRIANGLES, 0, mesh->GetVertexCount());
    } else {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->GetIndices().size()), GL_UNSIGNED_INT, nullptr);
    }
    VertexArray::UnbindVAO();
}
