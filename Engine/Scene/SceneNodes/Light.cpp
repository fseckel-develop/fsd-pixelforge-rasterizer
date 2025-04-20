#include "Light.h"
#include "../../Graphics/Pipeline/ShaderProgram.h"
#include "../../Geometry/Meshes/Mesh.h"


Light::Light(const string& name, const LightType type):
    SceneNode(name) {
    this->type = type;
}


void Light::SetAmbient(const vec3& color, const float intensity) {
    ambient = LightAttribute(color, intensity);
}


void Light::SetDiffuse(const vec3& color, const float intensity) {
    diffuse = LightAttribute(color, intensity);
}


void Light::SetSpecular(const vec3& color, const float intensity) {
    specular = LightAttribute(color, intensity);
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
