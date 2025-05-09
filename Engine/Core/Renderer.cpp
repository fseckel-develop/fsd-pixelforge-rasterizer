#include "../Core.h"
#include "../Scene.h"
#include "../Utilities.h"
#include "../Geometry/Meshes/Mesh.h"
#include "../Graphics/Pipeline/VertexArray.h"
#include "../Graphics/Pipeline/Shader.h"
#include "../Graphics/Texturing/Material.h"
using namespace std; using namespace glm;


unordered_map<string, shared_ptr<Shader>> Renderer::shaders;
shared_ptr<Shader> Renderer::currentShader = nullptr;
shared_ptr<Material> Renderer::fallbackMaterial = nullptr;
vector<shared_ptr<LightUnit>> Renderer::fallbackLightUnits;
bool Renderer::useFallbackLights = true;


void Renderer::Initialize() {
    InitializeShader("White");
    InitializeShader("Emissive");
    InitializeShader("Lighting");
    fallbackMaterial = make_shared<Material>(WhiteRubber());
    fallbackLightUnits = {
        LightUnit_("FallBack1").withLight(AmbientLight_()),
        LightUnit_("FallBack2").withLight(DirectionalLight_().withDirection({1.0f, -2.0f, -2.0f}))
    };
}


void Renderer::InitializeShader(const string& name) {
    const auto program = make_shared<Shader>();
    const string vertexShader = name + ".vert";
    const string fragmentShader = name + ".frag";
    program->AddModule(GL_VERTEX_SHADER, vertexShader.c_str());
    program->AddModule(GL_FRAGMENT_SHADER, fragmentShader.c_str());
    program->ActivateShader();
    shaders[name] = program;
}


void Renderer::PrepareFrame(const RenderMode mode, const vec4 clearColor) {
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, mode == WIREFRAME ? GL_LINE : GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}


void Renderer::SetShader(const string& name) { // NOLINT
    if (shaders.contains(name) && currentShader != shaders.at(name)) {
        Shader::UnbindShader();
        currentShader = shaders.at(name);
    }
}


void Renderer::SetCameraUniforms() { // NOLINT
    currentShader->SetUniform("viewMatrix", Camera::GetViewMatrix());
    currentShader->SetUniform("projectionMatrix", Camera::GetProjectionMatrix());
    currentShader->SetUniform("cameraPosition", Camera::GetPosition());
}


void Renderer::SetLightingUniforms(const vector<shared_ptr<LightUnit>>& lightNodes) { // NOLINT
    if (currentShader != shaders.at("Lighting")) SetShader("Lighting");
    currentShader->SetUniform("lightCount", static_cast<int>(lightNodes.size()));
    for (size_t i = 0; i < lightNodes.size(); i++) {
        auto light = lightNodes.at(i)->GetLight();
        currentShader->SetUniform(Utilities::UniformArray("lights", i, "type"), static_cast<int>(light->GetType()));
        currentShader->SetUniform(Utilities::UniformArray("lights", i, "ambient.color"), light->GetAmbient().color);
        currentShader->SetUniform(Utilities::UniformArray("lights", i, "ambient.intensity"), light->GetAmbient().intensity);
        currentShader->SetUniform(Utilities::UniformArray("lights", i, "diffuse.color"), light->GetDiffuse().color);
        currentShader->SetUniform(Utilities::UniformArray("lights", i, "diffuse.intensity"), light->GetDiffuse().intensity);
        currentShader->SetUniform(Utilities::UniformArray("lights", i, "specular.color"), light->GetSpecular().color);
        currentShader->SetUniform(Utilities::UniformArray("lights", i, "specular.intensity"), light->GetSpecular().intensity);
        switch (light->GetType()) {
            case Light::AMBIENT: break;
            case Light::DIRECTIONAL: {
                const auto directionalLight = dynamic_pointer_cast<DirectionalLight>(light);
                currentShader->SetUniform(Utilities::UniformArray("lights", i, "direction"), lightNodes.at(i)->GetCurrentDirection());
                break;
            }
            case Light::POSITIONAL: {
                const auto positionalLight = std::dynamic_pointer_cast<PositionalLight>(light);
                currentShader->SetUniform(Utilities::UniformArray("lights", i, "position"), lightNodes.at(i)->GetCurrentPosition());
                currentShader->SetUniform(Utilities::UniformArray("lights", i, "attenuation.constant"), positionalLight->GetAttenuation().constant);
                currentShader->SetUniform(Utilities::UniformArray("lights", i, "attenuation.linear"), positionalLight->GetAttenuation().linear);
                currentShader->SetUniform(Utilities::UniformArray("lights", i, "attenuation.quadratic"), positionalLight->GetAttenuation().quadratic);
                break;
            }
            case Light::SPOT: {
                const auto spotLight = std::dynamic_pointer_cast<SpotLight>(light);
                currentShader->SetUniform(Utilities::UniformArray("lights", i, "direction"), lightNodes.at(i)->GetCurrentDirection());
                currentShader->SetUniform(Utilities::UniformArray("lights", i, "position"), lightNodes.at(i)->GetCurrentPosition());
                currentShader->SetUniform(Utilities::UniformArray("lights", i, "attenuation.constant"), spotLight->GetAttenuation().constant);
                currentShader->SetUniform(Utilities::UniformArray("lights", i, "attenuation.linear"), spotLight->GetAttenuation().linear);
                currentShader->SetUniform(Utilities::UniformArray("lights", i, "attenuation.quadratic"), spotLight->GetAttenuation().quadratic);
                currentShader->SetUniform(Utilities::UniformArray("lights", i, "innerCutoff"), spotLight->GetInnerCutoff());
                currentShader->SetUniform(Utilities::UniformArray("lights", i, "outerCutoff"), spotLight->GetOuterCutoff());
                break;
            }
        }
    }
}


void Renderer::SetMaterialUniforms(const shared_ptr<Material>& material) { // NOLINT
    if (currentShader != shaders.at("Lighting")) SetShader("Lighting");
    if (material->GetDiffuseMap()) {
        currentShader->SetUniform("material.useDiffuseMap", true);
        currentShader->SetUniform("material.diffuseMap", material->GetDiffuseMap()->BindTexture());
    } else {
        currentShader->SetUniform("material.useDiffuseMap", false);
    }
    if (material->GetSpecularMap()) {
        currentShader->SetUniform("material.useSpecularMap", true);
        currentShader->SetUniform("material.specularMap", material->GetSpecularMap()->BindTexture());
    } else {
        currentShader->SetUniform("material.useSpecularMap", false);
    }
    currentShader->SetUniform("material.ambient", material->GetAmbient());
    currentShader->SetUniform("material.diffuse", material->GetDiffuse());
    currentShader->SetUniform("material.specular", material->GetSpecular());
    currentShader->SetUniform("material.shininess", material->GetShininess());
}


void Renderer::DrawMesh(const shared_ptr<Mesh>& mesh) { // NOLINT
    SetShader("White");
    SetCameraUniforms();
    currentShader->SetUniform("modelMatrix", mat4(1.0f));
    Draw(mesh);
}


void Renderer::DrawLightUnit(const shared_ptr<LightUnit>& lightUnit) { // NOLINT
    if (lightUnit->GetMesh()) {
        SetShader("Emissive");
        currentShader->SetUniform("modelMatrix", lightUnit->GetModelMatrix());
        currentShader->SetUniform("viewMatrix", Camera::GetViewMatrix());
        currentShader->SetUniform("projectionMatrix", Camera::GetProjectionMatrix());
        currentShader->SetUniform("emissive.color", lightUnit->GetLight()->GetAmbient().color);
        currentShader->SetUniform("emissive.intensity", lightUnit->GetLight()->GetAmbient().intensity);
        Draw(lightUnit->GetMesh());
    }
}


void Renderer::DrawRenderUnit(const shared_ptr<RenderUnit>& renderUnit) { // NOLINT
    SetShader("Lighting");
    SetCameraUniforms();
    currentShader->SetUniform("modelMatrix", renderUnit->GetModelMatrix());
    if (useFallbackLights) SetLightingUniforms(fallbackLightUnits);
    SetMaterialUniforms(renderUnit->GetMaterial() ? renderUnit->GetMaterial() : fallbackMaterial);
    Draw(renderUnit->GetMesh());
}


void Renderer::DrawModel(const shared_ptr<Model>& model) { // NOLINT
    for (const auto& lightUnit : model->GetLightUnits()) {
        DrawLightUnit(lightUnit);
    }
    useFallbackLights = model->GetLightUnits().empty();
    SetLightingUniforms(useFallbackLights ? fallbackLightUnits : model->GetLightUnits());
    for (const auto& renderUnit : model->GetRenderUnits()) {
        DrawRenderUnit(renderUnit);
    }
}



void Renderer::DrawScene(const shared_ptr<Scene>& scene) { // NOLINT
    for (const auto& lightUnit : scene->GetAllLightUnits()) {
        DrawLightUnit(lightUnit);
    }
    useFallbackLights = scene->GetAllLightUnits().empty();
    SetLightingUniforms(useFallbackLights ? fallbackLightUnits : scene->GetAllLightUnits());
    for (const auto& model : scene->GetModels()) {
        for (const auto& renderUnit : model->GetRenderUnits()) {
            DrawRenderUnit(renderUnit);
        }
    }
}


void Renderer::Draw(const shared_ptr<Mesh>& mesh) { // NOLINT
    currentShader->BindShader();
    mesh->GetVAO()->BindVAO();
    if (mesh->GetIndices().empty()) {
        glDrawArrays(GL_TRIANGLES, 0, mesh->GetVertexCount());
    } else {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->GetIndices().size()), GL_UNSIGNED_INT, nullptr);
    }
    VertexArray::UnbindVAO();
}
