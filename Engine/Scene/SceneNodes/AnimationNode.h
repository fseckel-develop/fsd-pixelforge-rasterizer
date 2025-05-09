#pragma once
#include <vector>
#include "TransformNode.h"
class Animation;


/// Extension of the TransformNode class with support of multiple
/// animations that contribute to the final transform.
class AnimationNode final : public TransformNode {
public:
    /// Constructs an animation node with a given name.
    /// @param name The name of the node.
    explicit AnimationNode(const std::string& name);

    /// Adds an animation to this node.
    /// @param animation The animation to add.
    void AddAnimation(const std::shared_ptr<Animation>& animation) override;

    /// Removes an animation from this node.
    /// @param animation The animation to remove.
    void RemoveAnimation(const std::shared_ptr<Animation>& animation);

    /// Starts playback of all attached animations.
    void PlayAllAnimations() const;

    /// Pauses all attached animations.
    void PauseAllAnimations() const;

    /// Resumes all attached animations.
    void ResumeAllAnimations() const;

    /// Stops all attached animations.
    void StopAllAnimations() const;

    /// Resets all attached animations to their initial state.
    void ResetAllAnimations() const;

    /// Updates all animations with the given time step.
    /// @param deltaTime Time step for animation updates.
    void UpdateAllAnimations(float deltaTime);

protected:
    /// Updates the node’s transform by applying all animations.
    /// @param deltaTime Time step passed from scene update.
    void UpdateSelf(float deltaTime) override;

private:
    std::vector<std::shared_ptr<Animation>> animations; ///< List of animations attached to this node.

    /// Invokes the given action on all attached animations.
    /// @param action Pointer to the animation method to call.
    void ForEachAnimation(void (Animation::*action)()) const;
};