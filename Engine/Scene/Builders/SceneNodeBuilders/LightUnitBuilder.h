#pragma once
#include "TransformNodeBuilder.h"
#include "../../SceneNodes/LightUnit.h"
using namespace std;


class LightUnitBuilder final : public TransformNodeBuilder<LightUnit, LightUnitBuilder> {
public:
    explicit LightUnitBuilder(const string& name, const string& parentName = ""):
        TransformNodeBuilder(name, parentName) {
    }

    auto& withLight(const shared_ptr<Light>& light) {
        this->sceneNode->SetLight(light);
        return *this;
    }

    template<IsLightBuilder BuilderT>
    auto& withLight(const BuilderT& builder) {
        this->sceneNode->SetLight(builder.Build());
        return *this;
    }

    auto& hasMesh(const shared_ptr<Mesh>& mesh) {
        this->sceneNode->SetMesh(mesh);
        return *this;
    }
};


inline auto LightUnit_(const string& name, const string& parentName = "") {
    return LightUnitBuilder(name, parentName);
}
