#pragma once
#include "AnimationBuilder.h"
#include "../../Animations/Rotating.h"


/// Extension of the AnimationBuilder class for constructing
/// rotating animations using a fluent interface.
class RotatingBuilder final : public AnimationBuilder<Rotating, RotatingBuilder> {
public:
    /// Starts the building process for a rotating animation in the given mode.
    /// @param mode The animation mode to use (LOOP, ONCE, BOUNCE).
    explicit RotatingBuilder(const Animation::Mode mode):
        AnimationBuilder(mode) {
    }

    /// Sets the axis of rotation for the animation.
    /// @param axis A unit vector representing the axis of rotation.
    /// @return Reference to this builder for method chaining.
    auto& withRotationAxis(const glm::vec3& axis) {
        animation->SetRotationAxis(axis);
        return *this;
    }

    /// Sets the total angle (in degrees) to rotate during the animation.
    /// @param angle Total angle in degrees.
    /// @return Reference to this builder for method chaining.
    auto& withTotalAngle(const float angle) {
        animation->SetTotalAngle(angle);
        return *this;
    }
};


/// Alias function to start building a rotating animation.
/// @param mode The animation mode to use (LOOP, ONCE, BOUNCE).
/// @return Rotating animation builder for fluent configuration.
inline auto Rotating_(const Animation::Mode mode) {
    return RotatingBuilder(mode);
}
