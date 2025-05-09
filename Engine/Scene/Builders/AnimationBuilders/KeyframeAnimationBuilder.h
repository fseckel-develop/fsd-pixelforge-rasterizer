#pragma once
#include "AnimationBuilder.h"
#include "../../Animations/KeyframeAnimation.h"


/// Extension of the AnimationBuilder class for building
/// keyframe animations using a fluent interface.
class KeyframeAnimationBuilder final : public AnimationBuilder<KeyframeAnimation, KeyframeAnimationBuilder> {
public:
    /// Starts the building process for a keyframe animation in the given mode.
    /// @param mode The animation mode to use (LOOP, ONCE, BOUNCE).
    explicit KeyframeAnimationBuilder(const Animation::Mode mode):
        AnimationBuilder(mode) {
    }

    /// Adds a keyframe to the animation at the given timestamp.
    /// @param timestamp The time at which the keyframe occurs.
    /// @param transform The transform to be applied at this keyframe.
    /// @return Reference to this builder for method chaining.
    auto& withKeyframe(const float timestamp, const Transform& transform) {
        animation->AddKeyframe(timestamp, transform);
        return *this;
    }
};


/// Alias function to begin building a KeyframeAnimation.
/// @param mode The animation mode to use (LOOP, ONCE, BOUNCE).
/// @return Keyframe animation builder for fluent configuration.
inline auto KeyframeAnimation_(const Animation::Mode mode) {
    return KeyframeAnimationBuilder(mode);
}
