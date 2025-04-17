#include "Model.h"
#include "../../Scene/SceneNodes/Renderable.h"
#include "../../Scene/SceneNodes/Light.h"


Model::Model(const string& name):
    SceneNode(name) {
}


void Model::AddLight(Light* light, const string& parent) {
    AddSceneNode(light, parent, lights);
}


void Model::AddRenderable(Renderable* renderable, const string& parent) {
    AddSceneNode(renderable, parent, renderables);
}


void Model::AddRenderable(const string& name, Mesh* mesh, const string& parent) {
    const auto renderable = new Renderable(name, mesh);
    AddRenderable(renderable, parent);
}


Light* Model::GetLight(const string& name) const {
    const auto it = lights.find(name);
    if (it != lights.end()) {
        return it->second;
    }
    return nullptr;
}


Renderable* Model::GetRenderable(const string& name) const {
    const auto it = renderables.find(name);
    if (it != renderables.end()) {
        return it->second;
    }
    return nullptr;
}


unordered_map<string, Light*>& Model::GetLights() {
    return lights;
}


unordered_map<string, Renderable*>& Model::GetRenderables() {
    return renderables;
}


SceneNode* Model::LastAdded() const {
    return lastAdded;
}


void Model::Update(const float deltaTime) {
    SceneNode::Update(deltaTime);
    for (auto& [_, renderable] : renderables) {
        renderable->Update(deltaTime);
    }
    for (auto& [_, light] : lights) {
        light->Update(deltaTime);
    }
}


template<typename T>
void Model::AddSceneNode(T* node, const string& parent, unordered_map<string, T*>& container) {
    if (!node) return;
    const string& nodeName = node->GetName();
    if (container.contains(nodeName)) {
        throw runtime_error("Node '" + nodeName + "' already exists in model '" + this->name + "'.");
    }
    container[nodeName] = node;
    if (!parent.empty()) {
        const auto it = container.find(parent);
        if (it == container.end()) {
            throw runtime_error("Parent '" + parent + "' for node '" + nodeName + "' not found in model '" + this->name + "'.");
        }
        it->second->AddChild(node);
    } else {
        AddChild(node);
    }
    lastAdded = node;
}