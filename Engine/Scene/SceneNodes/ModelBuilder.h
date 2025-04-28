#pragma once
#include "TransformNodeBuilder.h"
#include "LightUnitBuilder.h"
#include "RenderUnitBuilder.h"
#include "Model.h"
using namespace std;


class ModelBuilder : public TransformNodeBuilder<Model, ModelBuilder> {
public:
    explicit ModelBuilder(const string& name, const string& parentName = ""):
        TransformNodeBuilder(name, parentName) {
    }

    auto& With(const LightUnitBuilder& builder) {
        this->sceneNode->AddLightUnit(builder.Build());
        return *this;
    }

    auto& With(const RenderUnitBuilder& builder) {
        this->sceneNode->AddRenderUnit(builder.Build());
        return *this;
    }
};


inline auto Model_(const string& name) {
    return ModelBuilder(name);
}
