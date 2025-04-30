#include "LightUnit.h"
#include "../../Utilities.h"
#include "../../Managers/MeshManager.h"
#include "../../Managers/LightManager.h"
#include <GLM/gtc/quaternion.hpp>

#include "../Transforms/Translation.h"


LightUnit::LightUnit(const string& name):
    TransformNode(name) {
}


LightUnit::LightUnit(const string& name, const shared_ptr<Light>& light):
    TransformNode(name) {
    SetLight(light);
}


void LightUnit::SetLight(const shared_ptr<Light>& light) {
    this->light = LightManager::GetOrCreate(light);
    SetDefaultMesh(this->light->GetType());
}


void LightUnit::SetMesh(const shared_ptr<Mesh>& mesh) {
    if (mesh) {
        this->mesh = MeshManager::GetOrCreate(mesh);
        this->toBeRendered = true;
    } else {
        this->toBeRendered = false;
    }

}


const shared_ptr<Light>& LightUnit::GetLight() const {
    return light;
}


vec3 LightUnit::GetCurrentPosition() const {
    const auto position = vec3(GetGlobalTransform().ToMatrix() * vec4(light->GetPosition(), 1.0f));
    return position;
}


vec3 LightUnit::GetCurrentDirection() const {
    return normalize(mat3_cast(GetGlobalTransform().GetRotation()) * light->GetDirection());
}


mat4 LightUnit::GetModelMatrix() const {
    return (GetGlobalTransform() * nodeScale * Translation(light->GetPosition())).ToMatrix();
}


shared_ptr<Mesh>& LightUnit::GetMesh() {
    return mesh;
}


bool LightUnit::ToBeRendered() const {
    return toBeRendered;
}


// TODO: Orienting Cone and Arrow in the direction of the Light
// TODO: Cone and Arrow do show weird faulty rendering (faulty normals?)
void LightUnit::SetDefaultMesh(const Light::Type type) {
    switch (type) {
        case Light::DIRECTIONAL: {
            mesh = MeshManager::GetOrCreate(make_shared<Mesh>("Arrow.obj"));
            break;
        }
        case Light::POSITIONAL: {
            mesh = MeshManager::GetOrCreate(make_shared<Mesh>("Sphere.obj"));
            break;
        }
        case Light::SPOT: {
            mesh = MeshManager::GetOrCreate(make_shared<Mesh>("Cone.obj"));
            break;
        }
        default: break;
    }
    if (mesh) toBeRendered = true;
}

