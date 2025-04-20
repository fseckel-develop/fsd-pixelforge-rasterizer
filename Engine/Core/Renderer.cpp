#include "Renderer.h"
#include "Camera.h"
#include "Input.h"
#include "../Scene.h"
#include "../Utilities.h"
#include "../Geometry/Meshes/Mesh.h"
#include "../Graphics/Pipeline/ShaderProgram.h"
#include "../Graphics/Texturing/Material.h"



Renderer::Renderer() {
    InitializeShaderProgram("lighting");
    InitializeShaderProgram("emissive");
}


void Renderer::InitializeShaderProgram(const string& name) {
    const auto program = new ShaderProgram();
    const string vertexShader = name + ".vert";
    const string fragmentShader = name + ".frag";
    program->AddShader(GL_VERTEX_SHADER, vertexShader.c_str());
    program->AddShader(GL_FRAGMENT_SHADER, fragmentShader.c_str());
    program->ActivateProgram();
    programs[name] = program;
}


void Renderer::Render(const Scene& scene) const {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    scene.Update(Input::GetDeltaTime());
    ShaderProgram* emissiveProgram = programs.at("emissive");
    emissiveProgram->SetUniform("viewMatrix", Camera::GetViewMatrix());
    emissiveProgram->SetUniform("projectionMatrix", Camera::GetProjectionMatrix());
    for (const auto& light : scene.GetAllLights()) {
        if (light->ToBeRendered()) {
            emissiveProgram->SetUniform("modelMatrix", light->GetModelMatrix());
            emissiveProgram->SetUniform("emissive.color", light->GetAmbient().color);
            emissiveProgram->SetUniform("emissive.intensity", light->GetAmbient().intensity);
            emissiveProgram->UseProgram();
            light->GetMesh()->Render();
        }
    }
    ShaderProgram::UnuseProgram();
    ShaderProgram* lightingProgram = programs.at("lighting");
    lightingProgram->SetUniform("viewMatrix", Camera::GetViewMatrix());
    lightingProgram->SetUniform("projectionMatrix", Camera::GetProjectionMatrix());
    lightingProgram->SetUniform("cameraPosition", Camera::GetPosition());
    SetLighting(lightingProgram, scene.GetAllLights());
    for (const auto& model : scene.GetModels()) {
        for (const auto& renderUnit : model->GetRenderUnits()) {
            lightingProgram->SetUniform("modelMatrix", renderUnit->GetModelMatrix());
            if (renderUnit->GetMaterial()) {
                SetMaterial(lightingProgram, renderUnit->GetMaterial());
            }
            lightingProgram->UseProgram();
            renderUnit->GetMesh()->Render();
        }
    }
    ShaderProgram::UnuseProgram();
}


void Renderer::Render(const shared_ptr<Mesh>& mesh) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    mesh->Render();
}


void Renderer::SetLighting(ShaderProgram* program, const vector<shared_ptr<Light>>& lights) {
    program->SetUniform("lightCount", static_cast<int>(lights.size()));
    for (size_t i = 0; i < lights.size(); i++) {
        program->SetUniform(ArrayElement("lights", i, "type"), static_cast<int>(lights[i]->GetType()));
        program->SetUniform(ArrayElement("lights", i, "ambient.color"), lights[i]->GetAmbient().color);
        program->SetUniform(ArrayElement("lights", i, "ambient.intensity"), lights[i]->GetAmbient().intensity);
        program->SetUniform(ArrayElement("lights", i, "diffuse.color"), lights[i]->GetDiffuse().color);
        program->SetUniform(ArrayElement("lights", i, "diffuse.intensity"), lights[i]->GetDiffuse().intensity);
        program->SetUniform(ArrayElement("lights", i, "specular.color"), lights[i]->GetSpecular().color);
        program->SetUniform(ArrayElement("lights", i, "specular.intensity"), lights[i]->GetSpecular().intensity);
        switch (lights[i]->GetType()) {
            case AMBIENT: break;
            case DIRECTIONAL: {
                const auto light = dynamic_pointer_cast<DirectionalLight>(lights[i]);
                program->SetUniform(ArrayElement("lights", i, "direction"), light->GetCurrentDirection());
                break;
            }
            case POSITIONAL: {
                const auto light = dynamic_pointer_cast<PositionalLight>(lights[i]);
                program->SetUniform(ArrayElement("lights", i, "position"), light->GetCurrentPosition());
                program->SetUniform(ArrayElement("lights", i, "attenuation.constant"), light->GetAttenuation().constant);
                program->SetUniform(ArrayElement("lights", i, "attenuation.linear"), light->GetAttenuation().linear);
                program->SetUniform(ArrayElement("lights", i, "attenuation.quadratic"), light->GetAttenuation().quadratic);
                break;
            }
            case SPOT: {
                const auto light = dynamic_pointer_cast<SpotLight>(lights[i]);
                program->SetUniform(ArrayElement("lights", i, "direction"), light->GetCurrentDirection());
                program->SetUniform(ArrayElement("lights", i, "position"), light->GetCurrentPosition());
                program->SetUniform(ArrayElement("lights", i, "attenuation.constant"), light->GetAttenuation().constant);
                program->SetUniform(ArrayElement("lights", i, "attenuation.linear"), light->GetAttenuation().linear);
                program->SetUniform(ArrayElement("lights", i, "attenuation.quadratic"), light->GetAttenuation().quadratic);
                program->SetUniform(ArrayElement("lights", i, "innerCutoff"), light->GetInnerCutoff());
                program->SetUniform(ArrayElement("lights", i, "outerCutoff"), light->GetOuterCutoff());
                break;
            }
        }
    }
}


void Renderer::SetMaterial(ShaderProgram* program, const shared_ptr<Material>& material) {
    if (material->GetDiffuseMap()) {
        program->SetUniform("material.useDiffuseMap", true);
        program->SetUniform("material.diffuseMap", material->GetDiffuseMap()->BindTexture());
    }
    else {
        program->SetUniform("material.useDiffuseMap", false);
    }
    if (material->GetSpecularMap()) {
        program->SetUniform("material.useSpecularMap", true);
        program->SetUniform("material.specularMap", material->GetSpecularMap()->BindTexture());
    }
    else {
        program->SetUniform("material.useSpecularMap", false);
    }
    program->SetUniform("material.ambient", material->GetAmbient());
    program->SetUniform("material.diffuse", material->GetDiffuse());
    program->SetUniform("material.specular", material->GetSpecular());
    program->SetUniform("material.shininess", material->GetShininess());
}
