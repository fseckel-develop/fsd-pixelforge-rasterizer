#pragma once
#include "TransformNodeBuilder.h"
#include "../SceneNodes/RenderUnit.h"
using namespace std;


class RenderUnitBuilder : public TransformNodeBuilder<RenderUnit, RenderUnitBuilder> {
public:
    explicit RenderUnitBuilder(const string& name, const string& parentName = ""):
        TransformNodeBuilder(name, parentName) {
    }

    auto& withMesh(const shared_ptr<Mesh>& mesh) {
        this->sceneNode->SetMesh(mesh);
        return *this;
    }

    auto& withMaterial(const shared_ptr<Material>& material) {
        this->sceneNode->SetMaterial(material);
        return *this;
    }

    auto& WithTexture(const string& fileName) {
        this->sceneNode->SetTexture(fileName);
        return *this;
    }
};


inline auto RenderUnit_(const string& name, const string& parentName = "") {
    return RenderUnitBuilder(name, parentName);
}
