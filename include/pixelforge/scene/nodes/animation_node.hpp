#pragma once
#include <pixelforge/scene/nodes/transform_node.hpp>
#include <vector>

namespace pixelforge::scene::animation { class Animation; }

namespace pixelforge::scene::nodes {

    /// Extension of the TransformNode class with support of multiple
    /// animations that contribute to the final transform.
    class AnimationNode final : public TransformNode {
    public:
        /// Constructs an animation node with a given name.
        /// @param name The name of the node.
        explicit AnimationNode(const std::string& name);

        /// Adds an animation to this node.
        /// @param animation The animation to add.
        void addAnimation(const std::shared_ptr<animation::Animation>& animation) override;

        /// Removes an animation from this node.
        /// @param animation The animation to remove.
        void removeAnimation(const std::shared_ptr<animation::Animation>& animation);

        /// Starts playback of all attached animations.
        void playAllAnimations() const;

        /// Pauses all attached animations.
        void pauseAllAnimations() const;

        /// Resumes all attached animations.
        void resumeAllAnimations() const;

        /// Stops all attached animations.
        void stopAllAnimations() const;

        /// Resets all attached animations to their initial state.
        void resetAllAnimations() const;

        /// Updates all animations with the given time step.
        /// @param timeDelta Time step for animation updates.
        void updateAllAnimations(float timeDelta);

    protected:
        /// Updates the node’s transform by applying all animations.
        /// @param timeDelta Time step passed from scene update.
        void updateSelf(float timeDelta) override;

    private:
        std::vector<std::shared_ptr<animation::Animation>> animations_; ///< List of animations attached to this node.

        /// Invokes the given action on all attached animations.
        /// @param action Pointer to the animation method to call.
        void forEachAnimation(void (animation::Animation::*action)()) const;
    };

} // namespace pixelforge::scene::nodes
