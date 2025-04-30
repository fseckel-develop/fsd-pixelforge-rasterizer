#pragma once
#include "TransformNodeBuilder.h"
#include "../../SceneNodes/RenderUnit.h"
#include "../../../Geometry/Meshes/Mesh.h"
#include "../../../Graphics/Texturing/Material.h"


class RenderUnitBuilder final : public TransformNodeBuilder<RenderUnit, RenderUnitBuilder> {
public:
    explicit RenderUnitBuilder(const string& name, const string& parentName = ""):
        TransformNodeBuilder(name, parentName) {
    }

    auto& withMesh(const shared_ptr<Mesh>& mesh) {
        this->sceneNode->SetMesh(mesh);
        return *this;
    }

    auto& withMesh(const Mesh& mesh) {
        this->sceneNode->SetMesh(make_shared<Mesh>(mesh));
        return *this;
    }

    auto& withMesh(const string& fileName) {
        this->sceneNode->SetMesh(make_shared<Mesh>(fileName));
        return *this;
    }

    auto& withMaterial(const shared_ptr<Material>& material) {
        this->sceneNode->SetMaterial(material);
        return *this;
    }

    auto& withMaterial(const Material& material) {
        this->sceneNode->SetMaterial(make_shared<Material>(material));
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
