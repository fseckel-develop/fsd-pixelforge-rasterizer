#include "Model.h"
#include "../../Scene/SceneNodes/LightNode.h"
#include "../../Scene/SceneNodes/RenderUnit.h"


Model::Model(const string& name):
    TransformNode(name) {
}


void Model::AddLight(const string& name, const shared_ptr<Light>& light, const string& parentName) {
    AddSceneNode(make_shared<LightNode>(name, light), parentName, lights);
}


void Model::AddRenderUnit(const string& name, const shared_ptr<Mesh>& mesh, const string& parentName) {
    AddSceneNode(make_shared<RenderUnit>(name, mesh), parentName, renderUnits);
}


shared_ptr<LightNode> Model::GetLightByName(const string& name) const {
    for (const auto& lightNode : lights) {
        if (lightNode && lightNode->GetName() == name) return lightNode;
    }
    return nullptr;
}


shared_ptr<RenderUnit> Model::GetRenderUnitByName(const string& name) const {
    for (const auto& renderUnit : renderUnits) {
        if (renderUnit && renderUnit->GetName() == name) return renderUnit;
    }
    return nullptr;
}


vector<shared_ptr<LightNode>>& Model::GetLights() {
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
