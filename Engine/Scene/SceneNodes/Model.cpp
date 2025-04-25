#include "Model.h"
#include "../../Scene/SceneNodes/Light.h"
#include "../../Scene/SceneNodes/RenderUnit.h"


Model::Model(const string& name):
    TransformNode(name) {
}


void Model::AddLight(Light* light, const string& parentName) {
    AddSceneNode(shared_ptr<Light>(light), parentName, lights);
}


void Model::AddRenderUnit(RenderUnit* renderUnit, const string& parentName) {
    AddSceneNode(shared_ptr<RenderUnit>(renderUnit), parentName, renderUnits);
}


shared_ptr<Light> Model::GetLightByName(const string& name) const {
    for (const auto& light : lights) {
        if (light && light->GetName() == name) return light;
    }
    return nullptr;
}


shared_ptr<RenderUnit> Model::GetRenderUnitByName(const string& name) const {
    for (const auto& renderUnit : renderUnits) {
        if (renderUnit && renderUnit->GetName() == name) return renderUnit;
    }
    return nullptr;
}


vector<shared_ptr<Light>>& Model::GetLights() {
    return lights;
}


vector<shared_ptr<RenderUnit>>& Model::GetRenderUnits() {
    return renderUnits;
}


shared_ptr<SceneNode> Model::FindSceneNodeByName(const string& name) const {
    for (const auto& light : lights) {
        if (light && light->GetName() == name) return light;
    }
    for (const auto& unit : renderUnits) {
        if (unit && unit->GetName() == name) return unit;
    }
    return nullptr;
}
