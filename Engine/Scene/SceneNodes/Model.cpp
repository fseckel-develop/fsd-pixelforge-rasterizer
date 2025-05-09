#include "Model.h"
#include "LightUnit.h"
#include "RenderUnit.h"
using namespace std;


Model::Model(const string& name):
    TransformNode(name) {
}


void Model::AddLightUnit(const shared_ptr<LightUnit>& lightUnit) {
    lightUnits.push_back(lightUnit);
    if (const auto root = lightUnit->GetRoot()) {
        AddChild(root);
    } else {
        AddChild(lightUnit);
    }
}


void Model::AddRenderUnit(const shared_ptr<RenderUnit>& renderUnit) {
    renderUnits.push_back(renderUnit);
    if (const auto root = renderUnit->GetRoot()) {
        AddChild(root);
    } else {
        AddChild(renderUnit);
    }
}


shared_ptr<LightUnit> Model::GetLightUnitByName(const string& name) const {
    for (const auto& lightUnit : lightUnits) {
        if (lightUnit && lightUnit->GetName() == name) return lightUnit;
    }
    return nullptr;
}


shared_ptr<RenderUnit> Model::GetRenderUnitByName(const string& name) const {
    for (const auto& renderUnit : renderUnits) {
        if (renderUnit && renderUnit->GetName() == name) return renderUnit;
    }
    return nullptr;
}


const vector<shared_ptr<LightUnit>>& Model::GetLightUnits() {
    return lightUnits;
}


const vector<shared_ptr<RenderUnit>>& Model::GetRenderUnits() {
    return renderUnits;
}
