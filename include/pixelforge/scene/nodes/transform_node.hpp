#pragma once
#include <pixelforge/scene/nodes/scene_node.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <pixelforge/scene/transform/scale.hpp>

namespace pixelforge::scene::animation { class Animation; }

namespace pixelforge::scene::nodes {

    /// Extension of the SceneNode class, enabling hierarchical transformations (translation,
    /// rotation and scale) as well as a separate node scale only affecting the current node.
    class TransformNode : public SceneNode {
    public:
        /// Constructs a transform node with a given name.
        /// @param name The name of the node.
        explicit TransformNode(const std::string& name);

        /// Sets the local transform of this node.
        /// @param transform The new local transform.
        void setTransform(const transform::Transform& transform);

        /// Sets a uniform scale to be applied only to this node.
        /// @param scale The scalar value for scaling.
        void setNodeScale(float scale);

        /// Sets a non-uniform scale vector to be applied only to this node.
        /// @param scale The scale vector.
        void setNodeScale(const glm::vec3& scale);

        /// Marks this node and its transform descendants as needing a global transform update.
        void markGlobalTransformDirty();

        /// Adds an animation to this node, wrapping it in an animation node if needed.
        /// The animation node then is set as the parent of the transform node.
        /// @param animation The animation to be added.
        virtual void addAnimation(const std::shared_ptr<animation::Animation>& animation);

        /// Checks whether the global transform is outdated.
        /// @return True if dirty, false if up to date.
        [[nodiscard]] bool isGlobalTransformDirty() const;

        /// Gets the current global transform.
        /// @return The global transform combining this node and its ancestors.
        [[nodiscard]] const transform::Transform& getGlobalTransform() const;

        /// Gets the model matrix for this node.
        /// @return 4x4 matrix combining global transform and node scale.
        [[nodiscard]] virtual glm::mat4 getModelMatrix() const;

    protected:
        transform::Scale nodeScale_; ///< Scale applied only to this node.
        transform::Transform localTransform_; ///< Local transformation of the node.
        mutable transform::Transform globalTransform_; ///< Cached global transformation (from root to this node).
        mutable bool globalTransformDirty_ = true; ///< Indicates if the global transform needs recomputation.

        /// Gets the current local transform.
        /// @return The local transform of this node.
        [[nodiscard]] virtual transform::Transform getLocalTransform() const;

        /// Updates this node’s transform state.
        void updateSelf(float /*timeDelta*/) override;

        /// Computes the global transform by walking up the ancestor chain.
        void updateGlobalTransform() const;

        /// Finds the next transform node ancestor of a given node.
        /// @param node Starting node to search from.
        /// @return Closest ancestor transform node or nullptr.
        static std::shared_ptr<TransformNode> findNextTransformAncestorFrom(const std::shared_ptr<SceneNode>& node);
    };

} // namespace pixelforge::scene::nodes
