#include "Renderer.h"
#include "../Graphics/Pipeline/ShaderProgram.h"
#include "../Scene/Lights/DirectionalLight.h"
#include "../Scene/Lights/PositionalLight.h"
#include "../Scene/Lights/SpotLight.h"
#include "../Graphics/Texturing/Material.h"
#include "../Utilities.h"


void Renderer::UploadMaterialToShader(const Material& material, ShaderProgram& program) const {
    if (material.GetDiffuseMap()) {
        program.SetUniform("material.useDiffuseMap", true);
        program.SetUniform("material.diffuseMap", material.GetDiffuseMap()->BindTexture());
    }
    else {
        program.SetUniform("material.useDiffuseMap", false);
    }
    if (material.GetSpecularMap()) {
        program.SetUniform("material.useSpecularMap", true);
        program.SetUniform("material.specularMap", material.GetSpecularMap()->BindTexture());
    }
    else {
        program.SetUniform("material.useSpecularMap", false);
    }
    program.SetUniform("material.ambient", material.GetAmbient());
    program.SetUniform("material.diffuse", material.GetDiffuse());
    program.SetUniform("material.specular", material.GetSpecular());
    program.SetUniform("material.shininess", material.GetShininess());
}


void Renderer::UploadLightsToShader(const vector<Light*>& lights, ShaderProgram& program) const {
    program.SetUniform("lightCount", static_cast<int>(lights.size()));
    for (size_t i = 0; i < lights.size(); i++) {
        program.SetUniform(ArrayElement("lights", i, "type"), static_cast<int>(lights[i]->GetType()));
        program.SetUniform(ArrayElement("lights", i, "ambient.color"), lights[i]->GetAmbient().color);
        program.SetUniform(ArrayElement("lights", i, "ambient.intensity"), lights[i]->GetAmbient().intensity);
        program.SetUniform(ArrayElement("lights", i, "diffuse.color"), lights[i]->GetDiffuse().color);
        program.SetUniform(ArrayElement("lights", i, "diffuse.intensity"), lights[i]->GetDiffuse().intensity);
        program.SetUniform(ArrayElement("lights", i, "specular.color"), lights[i]->GetSpecular().color);
        program.SetUniform(ArrayElement("lights", i, "specular.intensity"), lights[i]->GetSpecular().intensity);
        switch (lights[i]->GetType()) {
            case AMBIENT: break;
            case DIRECTIONAL: {
                const auto light = dynamic_cast<DirectionalLight*>(lights[i]);
                program.SetUniform(ArrayElement("lights", i, "direction"), light->GetCurrentDirection());
                break;
            }
            case POSITIONAL: {
                const auto light = dynamic_cast<PositionalLight*>(lights[i]);
                program.SetUniform(ArrayElement("lights", i, "position"), light->GetCurrentPosition());
                program.SetUniform(ArrayElement("lights", i, "attenuation.constant"), light->GetAttenuation().constant);
                program.SetUniform(ArrayElement("lights", i, "attenuation.linear"), light->GetAttenuation().linear);
                program.SetUniform(ArrayElement("lights", i, "attenuation.quadratic"), light->GetAttenuation().quadratic);
                break;
            }
            case SPOT: {
                const auto light = dynamic_cast<SpotLight*>(lights[i]);
                program.SetUniform(ArrayElement("lights", i, "direction"), light->GetCurrentDirection());
                program.SetUniform(ArrayElement("lights", i, "position"), light->GetCurrentPosition());
                program.SetUniform(ArrayElement("lights", i, "attenuation.constant"), light->GetAttenuation().constant);
                program.SetUniform(ArrayElement("lights", i, "attenuation.linear"), light->GetAttenuation().linear);
                program.SetUniform(ArrayElement("lights", i, "attenuation.quadratic"), light->GetAttenuation().quadratic);
                program.SetUniform(ArrayElement("lights", i, "innerCutoff"), light->GetInnerCutoff());
                program.SetUniform(ArrayElement("lights", i, "outerCutoff"), light->GetOuterCutoff());
                break;
            }
        }
    }
}



