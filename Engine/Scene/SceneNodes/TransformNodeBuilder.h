#pragma once
#include "SceneNodeBuilder.h"


template<typename NodeT, typename DerivedBuilder>
class TransformNodeBuilder : public SceneNodeBuilder<NodeT, DerivedBuilder> {
public:
    using SceneNodeBuilder<NodeT, DerivedBuilder>::SceneNodeBuilder;

    auto& withTransform(const Transform& transform) {
        this->sceneNode->SetTransform(transform);
        return static_cast<DerivedBuilder&>(*this);
    }

    auto& withNodeScale(const vec3& nodeScale) {
        this->sceneNode->SetNodeScale(nodeScale);
        return static_cast<DerivedBuilder&>(*this);
    }

    auto& withAnimation(const shared_ptr<Animation>& animation) {
        this->sceneNode->AddAnimation(animation);
        return static_cast<DerivedBuilder&>(*this);
    }
};
