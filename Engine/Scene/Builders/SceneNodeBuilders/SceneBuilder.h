#pragma once
#include "GroupBuilder.h"
#include "ModelBuilder.h"
#include "LightUnitBuilder.h"
#include "../../SceneNodes/Scene.h"


class SceneBuilder final : public GroupBuilder<Scene, SceneBuilder> {
public:
    explicit SceneBuilder(const string& name):
        GroupBuilder(name) {
    }

    auto& With(const LightUnitBuilder& builder) {
        this->sceneNode->AddLightUnit(builder.Build());
        return *this;
    }

    auto& With(const ModelBuilder& builder) {
        this->sceneNode->AddModel(builder.Build());
        return *this;
    }
};


inline auto Scene_(const string& name) {
    return SceneBuilder(name);
}
