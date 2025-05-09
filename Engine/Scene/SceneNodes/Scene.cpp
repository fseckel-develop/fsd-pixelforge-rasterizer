#include "Scene.h"
#include "Model.h"
#include "LightUnit.h"
using namespace std;


Scene::Scene(const string& name):
    Group(name) {
}


void Scene::AddModel(const shared_ptr<Model>& model) {
    if (!model) return;
    models.push_back(model);
    if (const auto root = model->GetRoot()) {
        AddChild(root);
    } else {
        AddChild(model);
    }
}


void Scene::AddLightUnit(const shared_ptr<LightUnit>& lightUnit) {
    if (!lightUnit) return;
    globalLightUnits.push_back(lightUnit);
    if (const auto root = lightUnit->GetRoot()) {
        AddChild(root);
    } else {
        AddChild(lightUnit);
    }
}


const vector<shared_ptr<Model>>& Scene::GetModels() const {
    return models;
}


const vector<shared_ptr<LightUnit>>& Scene::GetGlobalLightUnits() const {
    return globalLightUnits;
}


vector<shared_ptr<LightUnit>> Scene::GetAllLightUnits() const {
    vector<shared_ptr<LightUnit>> allLights = GetGlobalLightUnits();
    for (const auto& model : models) {
        for (const auto& lightUnit : model->GetLightUnits()) allLights.push_back(lightUnit);
    }
    return allLights;
}
