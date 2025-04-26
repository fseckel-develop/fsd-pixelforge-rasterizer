#include "LightNode.h"
#include "../../Managers/MeshManager.h"
#include "../../Managers/LightManager.h"
#include <GLM/gtc/quaternion.hpp>


LightNode::LightNode(const string& name, const shared_ptr<Light>& light):
    TransformNode(name) {
    this->light = LightManager::GetOrCreate(light);
}


void LightNode::SetMesh(const shared_ptr<Mesh>& mesh) {
    this->mesh = MeshManager::GetOrCreate(mesh);
    this->toBeRendered = true;
}


const shared_ptr<Light>& LightNode::GetLight() const {
    return light;
}


vec3 LightNode::GetCurrentPosition() const {
    const auto position = vec3(GetGlobalTransform().ToMatrix() * vec4(light->GetPosition(), 1.0f));
    return position; // NOLINT
}


vec3 LightNode::GetCurrentDirection() const {
    return normalize(mat3_cast(GetGlobalTransform().rotation) * light->GetDirection());
}


shared_ptr<Mesh>& LightNode::GetMesh() {
    return mesh;
}


bool LightNode::ToBeRendered() const {
    return toBeRendered;
}
