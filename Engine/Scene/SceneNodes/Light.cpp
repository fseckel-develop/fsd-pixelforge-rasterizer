#include "Light.h"
#include "../../Graphics/Pipeline/ShaderProgram.h"
#include "../../Geometry/Meshes/Mesh.h"


Light::Light(const string& name, const LightType type, const vec3& color, const float intensity):
    SceneNode(name) {
    this->type = type;
    SetColor(color);
    SetIntensity(intensity);
}


void Light::SetColor(const vec3& color) {
    const vec3 clamped = { glm::clamp(color.r, 0.0f, 1.0f), glm::clamp(color.g, 0.0f, 1.0f), glm::clamp(color.b, 0.0f, 1.0f) };
    ambient.color = clamped;
    diffuse.color = clamped;
    specular.color = clamped;
}


void Light::SetIntensity(const float intensity) {
    const float clamped = glm::clamp(intensity, 0.0f, 1.0f);
    ambient.intensity = clamped;
    diffuse.intensity = clamped;
    specular.intensity = clamped;
}


void Light::SetAmbient(const vec3& color, const float intensity) {
    const vec3 clampedColor = { glm::clamp(color.r, 0.0f, 1.0f), glm::clamp(color.g, 0.0f, 1.0f), glm::clamp(color.b, 0.0f, 1.0f) };
    const float clampedIntensity = glm::clamp(intensity, 0.0f, 1.0f);
    ambient = LightAttribute(clampedColor, clampedIntensity);
}


void Light::SetDiffuse(const vec3& color, const float intensity) {
    const vec3 clampedColor = { glm::clamp(color.r, 0.0f, 1.0f), glm::clamp(color.g, 0.0f, 1.0f), glm::clamp(color.b, 0.0f, 1.0f) };
    const float clampedIntensity = glm::clamp(intensity, 0.0f, 1.0f);
    diffuse = LightAttribute(clampedColor, clampedIntensity);
}


void Light::SetSpecular(const vec3& color, const float intensity) {
    const vec3 clampedColor = { glm::clamp(color.r, 0.0f, 1.0f), glm::clamp(color.g, 0.0f, 1.0f), glm::clamp(color.b, 0.0f, 1.0f) };
    const float clampedIntensity = glm::clamp(intensity, 0.0f, 1.0f);
    specular = LightAttribute(clampedColor, clampedIntensity);
}


void Light::SetMesh(Mesh* mesh) {
    if (mesh) {
        this->mesh = shared_ptr<Mesh>(mesh);
        toBeRendered = true;
    }
    else {
        toBeRendered = false;
    }
}



LightType Light::GetType() const {
    return type;
}


LightAttribute Light::GetAmbient() const {
    return ambient;
}


LightAttribute Light::GetDiffuse() const {
    return diffuse;
}


LightAttribute Light::GetSpecular() const {
    return specular;
}


vec3 Light::GetCurrentPosition() const {
    return { 0.0f, 0.0f, 0.0f };
}


vec3 Light::GetCurrentDirection() const {
    return { 0.0f, 0.0f, 0.0f };
}


bool Light::ToBeRendered() const {
    return toBeRendered;
}


shared_ptr<Mesh>& Light::GetMesh() {
    return mesh;
}
