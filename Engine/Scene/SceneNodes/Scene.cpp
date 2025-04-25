#include "Scene.h"
#include "Model.h"
#include "Light.h"


Scene::Scene(const string& name):
    Group(name) {
}


void Scene::AddModel(const shared_ptr<Model>& model) const {
    if (model) models->AddChild(model);
}


void Scene::AddLight(const shared_ptr<Light>& light) const {
    if (light) globalLights->AddChild(light);
}


vector<shared_ptr<Model>> Scene::GetModels() const {
    vector<shared_ptr<Model>> models;
    for (const auto& child : this->models->GetChildren()) {
        if (const auto model = dynamic_pointer_cast<Model>(child)) models.push_back(model);
    }
    return models;
}


vector<shared_ptr<Light>> Scene::GetGlobalLights() const {
    vector<shared_ptr<Light>> globalLights;
    for (const auto& child : this->globalLights->GetChildren()) {
        if (const auto light = dynamic_pointer_cast<Light>(child)) globalLights.push_back(light);
    }
    return globalLights;
}


vector<shared_ptr<Light>> Scene::GetAllLights() const {
    vector<shared_ptr<Light>> allLights = GetGlobalLights();
    for (const auto& child : models->GetChildren()) {
        if (const auto model = dynamic_pointer_cast<Model>(child)) {
            for (const auto& light : model->GetLights()) allLights.push_back(light);
        }
    }
    return allLights;
}


void Scene::UpdateSelf(const float deltaTime) {
    models->UpdateSelfAndChildren(deltaTime);
    globalLights->UpdateSelfAndChildren(deltaTime);
}
