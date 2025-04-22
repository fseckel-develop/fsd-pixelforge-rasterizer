#include "Scene.h"
#include "SceneNodes/Model.h"
#include "SceneNodes/Light.h"
#include "../Geometry/Meshes/Mesh.h"


void Scene::AddModel(const shared_ptr<Model>& model) {
    if (model) {
        this->models.push_back(model);
    }
}


void Scene::AddLight(const shared_ptr<Light>& light) {
    if (light) {
        this->globalLights.push_back(light);
    }
}


const vector<shared_ptr<Model>>& Scene::GetModels() const {
    return models;
}


const vector<shared_ptr<Light>>& Scene::GetGlobalLights() const {
    return globalLights;
}


vector<shared_ptr<Light>> Scene::GetAllLights() const {
    vector<shared_ptr<Light>> allLights;
    allLights.reserve(globalLights.size() + models.size() * 4);
    allLights.insert(allLights.end(), globalLights.begin(), globalLights.end());
    for (const auto& model : models) {
        for (const auto& light : model->GetLights()) {
            allLights.push_back(light);
        }
    }
    return allLights;
}


void Scene::Update(const float deltaTime) const {
    for (const auto& model : models) {
        model->Update(deltaTime);
    }
    for (const auto& light : globalLights) {
        light->Update(deltaTime);
    }
}


void Scene::Clear() {
    this->models.clear();
    this->globalLights.clear();
}
