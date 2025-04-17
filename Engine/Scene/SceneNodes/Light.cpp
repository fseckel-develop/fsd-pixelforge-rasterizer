#include "Light.h"
#include "../../Graphics/Pipeline/ShaderProgram.h"
#include "../../Geometry/Meshes/Mesh.h"


Light::Light(const string& name, const LightType type):
    SceneNode(name) {
    this->type = type;
}


void Light::SetAmbient(const vec3& color, const float intensity) {
    this->ambient = LightAttribute(color, intensity);
}


void Light::SetDiffuse(const vec3& color, const float intensity) {
    this->diffuse = LightAttribute(color, intensity);
}


void Light::SetSpecular(const vec3& color, const float intensity) {
    this->specular = LightAttribute(color, intensity);
}


void Light::SetMesh(Mesh* mesh) {
    if (mesh) {
        this->mesh = mesh;
        this->toBeRendered = true;
    }
    else {
        this->toBeRendered = false;
    }
}



LightType Light::GetType() const {
    return type;
}


LightAttribute Light::GetAmbient() const {
    return this->ambient;
}


LightAttribute Light::GetDiffuse() const {
    return this->diffuse;
}


LightAttribute Light::GetSpecular() const {
    return this->specular;
}


vec3 Light::GetCurrentPosition() const {
    return { 0.0f, 0.0f, 0.0f };
}


vec3 Light::GetCurrentDirection() const {
    return { 0.0f, 0.0f, 0.0f };
}


bool Light::ToBeRendered() const {
    return this->toBeRendered;
}


// TODO: Own ShaderProgram to render lights!
void Light::Render(ShaderProgram& program, const Transform& globalTransform) const {
    if (mesh && toBeRendered) {
        program.SetUniform("modelMatrix", globalTransform.ToMatrix() * GetModelMatrix());
        program.UseProgram();
        mesh->Render();
    }
}
