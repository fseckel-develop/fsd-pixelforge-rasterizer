#pragma once
#include "SceneNodeBuilder.h"
#include "../TransformBuilder.h"
#include "../../SceneNodes/TransformNode.h"


template<typename NodeT, typename DerivedBuilder>
class TransformNodeBuilder : public SceneNodeBuilder<NodeT, DerivedBuilder> {
public:
    explicit TransformNodeBuilder(const string& name, const string& parentName = ""):
        SceneNodeBuilder<NodeT, DerivedBuilder>(name, parentName) {
    }

    auto& withTransform(const Transform& transform) {
        this->sceneNode->SetTransform(transform);
        return static_cast<DerivedBuilder&>(*this);
    }

    auto& withTransform(const TransformBuilder& builder) {
        this->sceneNode->SetTransform(builder.Build());
        return static_cast<DerivedBuilder&>(*this);
    }

    auto& withNodeScale(const float nodeScale) {
        this->sceneNode->SetNodeScale(nodeScale);
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

    template<IsAnimationBuilder BuilderT>
    auto& withAnimation(const BuilderT& builder) {
        this->sceneNode->AddAnimation(builder.Build());
        return static_cast<DerivedBuilder&>(*this);
    }
};
