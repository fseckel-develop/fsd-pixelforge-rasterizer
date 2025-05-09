#pragma once
#include "SceneNodeBuilder.h"
#include "../TransformBuilder.h"
#include "../../SceneNodes/TransformNode.h"


/// Extension of the SceneNodeBuilder class for building
/// a transform node using a fluent interface.
/// @tparam NodeT The specific TransformNode subclass to build.
/// @tparam DerivedBuilder The concrete builder class inheriting from this template.
template<typename NodeT, typename DerivedBuilder>
class TransformNodeBuilder : public SceneNodeBuilder<NodeT, DerivedBuilder> {
public:
    /// Starts the building process for a transform node with a name and parent (optional).
    /// @param name The name of the transform node.
    /// @param parentName The name of the parent node (optional).
    explicit TransformNodeBuilder(const std::string& name, const std::string& parentName = ""):
        SceneNodeBuilder<NodeT, DerivedBuilder>(name, parentName) {
    }

    /// Sets the transform for the transform node being built.
    /// @param transform The transform to be applied to the node.
    /// @return Reference to the derived builder for fluent chaining.
    auto& withTransform(const Transform& transform) {
        this->sceneNode->SetTransform(transform);
        return static_cast<DerivedBuilder&>(*this);
    }

    /// Sets the transform for the transform node using a transform builder.
    /// @param builder Transform builder used to construct the transform.
    /// @return Reference to the derived builder for fluent chaining.
    auto& withTransform(const TransformBuilder& builder) {
        this->sceneNode->SetTransform(builder.Build());
        return static_cast<DerivedBuilder&>(*this);
    }

    /// Sets a uniform node scale for the transform node being built.
    /// @param nodeScale The scale value to be applied to the node.
    /// @return Reference to the derived builder for fluent chaining.
    auto& withNodeScale(const float nodeScale) {
        this->sceneNode->SetNodeScale(nodeScale);
        return static_cast<DerivedBuilder&>(*this);
    }

    /// Sets a non-uniform node scale for the transform node being built.
    /// @param nodeScale The scale vector to be applied to the node.
    /// @return Reference to the derived builder for fluent chaining.
    auto& withNodeScale(const glm::vec3& nodeScale) {
        this->sceneNode->SetNodeScale(nodeScale);
        return static_cast<DerivedBuilder&>(*this);
    }

    /// Applies an animation to the transform node being
    /// built, by attaching it to a parent animation node.
    /// @param animation The animation to be added.
    /// @return Reference to the derived builder for fluent chaining.
    auto& withAnimation(const std::shared_ptr<Animation>& animation) {
        this->sceneNode->AddAnimation(animation);
        return static_cast<DerivedBuilder&>(*this);
    }

    /// Adds an animation to the transform node using an animation builder.
    /// @tparam BuilderT Animation builder used to construct the animation.
    /// @return Reference to the derived builder for fluent chaining.
    template<IsAnimationBuilder BuilderT>
    auto& withAnimation(const BuilderT& builder) {
        this->sceneNode->AddAnimation(builder.Build());
        return static_cast<DerivedBuilder&>(*this);
    }
};
