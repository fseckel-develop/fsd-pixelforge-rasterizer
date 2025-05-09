#pragma once
#include "SceneNode.h"
#include "../Transforms/Transform.h"
#include "../Transforms/Scale.h"
class Animation;


/// Extension of the SceneNode class, enabling hierarchical transformations (translation,
/// rotation and scale) as well as a separate node scale only affecting the current node.
class TransformNode : public SceneNode {
public:
    /// Constructs a transform node with a given name.
    /// @param name The name of the node.
    explicit TransformNode(const std::string& name);

    /// Sets the local transform of this node.
    /// @param transform The new local transform.
    void SetTransform(const Transform& transform);

    /// Sets a uniform scale to be applied only to this node.
    /// @param scale The scalar value for scaling.
    void SetNodeScale(float scale);

    /// Sets a non-uniform scale vector to be applied only to this node.
    /// @param scale The scale vector.
    void SetNodeScale(const glm::vec3& scale);

    /// Marks this node and its transform descendants as needing a global transform update.
    void MarkGlobalTransformDirty();

    /// Adds an animation to this node, wrapping it in an animation node if needed.
    /// @param animation The animation to be added.
    virtual void AddAnimation(const std::shared_ptr<Animation>& animation);

    /// Checks whether the global transform is outdated.
    /// @return True if dirty, false if up to date.
    [[nodiscard]] bool IsGlobalTransformDirty() const;

    /// Gets the current global transform.
    /// @return The global transform combining this node and its ancestors.
    [[nodiscard]] const Transform& GetGlobalTransform() const;

    /// Gets the model matrix for this node.
    /// @return 4x4 matrix combining global transform and node scale.
    [[nodiscard]] virtual glm::mat4 GetModelMatrix() const;

protected:
    Scale nodeScale; ///< Scale applied only to this node.
    Transform localTransform; ///< Local transformation of the node.
    mutable Transform globalTransform; ///< Cached global transformation (from root to this node).
    mutable bool globalTransformDirty = true; ///< Indicates if the global transform needs recomputation.

    /// Updates this node’s transform state.
    void UpdateSelf(float /*deltaTime*/) override;

    /// Finds the next transform node ancestor of a given node.
    /// @param node Starting node to search from.
    /// @return Closest ancestor transform node or nullptr.
    static std::shared_ptr<TransformNode> FindNextTransformAncestorFrom(const std::shared_ptr<SceneNode>& node);

private:
    /// Computes the global transform by walking up the ancestor chain.
    void UpdateGlobalTransform() const;
};
