#include "Scene.h"
#include "SceneNodes/Model.h"
#include "SceneNodes/Light.h"
#include "SceneNodes/Renderable.h"


void Scene::AddModel(Model* model) {
    if (model) {
        this->models.push_back(model);
    }
}


void Scene::AddLight(Light* light) {
    if (light) {
        this->globalLights.push_back(light);
    }
}


void Scene::AddRenderable(Renderable* renderable) {
    if (renderable) {
        this->globalRenderables.push_back(renderable);
    }
}


vector<Light *> Scene::GetAllLights() const {
    vector<Light*> allLights;
    allLights.reserve(globalLights.size() + models.size() * 4); // rough guess
    allLights.insert(allLights.end(), globalLights.begin(), globalLights.end());
    for (const auto& model : models) {
        for (const auto& [_, light] : model->GetLights()) {
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
    for (const auto& renderable : globalRenderables) {
        renderable->Update(deltaTime);
    }
}


void Scene::Clear() {
    this->models.clear();
    this->globalLights.clear();
    this->globalRenderables.clear();
}
