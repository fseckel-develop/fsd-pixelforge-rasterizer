#pragma once
#include "AnimationBuilder.h"
#include "../../Animations/Translating.h"


/// Extension of the AnimationBuilder class for building
///translating animations using a fluent interface.
class TranslatingBuilder final : public AnimationBuilder<Translating, TranslatingBuilder> {
public:
    /// Starts the building process for a translating animation in the given mode.
    /// @param mode The animation mode to use (LOOP, ONCE, BOUNCE).
    explicit TranslatingBuilder(const Animation::Mode mode):
        AnimationBuilder(mode) {
    }

    /// Sets the direction of animated translation.
    /// @param direction 3D vector direction to move along.
    /// @return Reference to this builder for method chaining.
    auto& withDirection(const glm::vec3& direction) {
        animation->SetDirection(direction);
        return *this;
    }

    /// Sets the total distance to translate over the animated duration.
    /// @param distance Total distance in world units.
    /// @return Reference to this builder for method chaining.
    auto& withTotalDistance(const float distance) {
        animation->SetTotalDistance(distance);
        return *this;
    }
};


/// Alias function to start building a translating animation.
/// @param mode The animation mode to use (LOOP, ONCE, BOUNCE).
/// @return Translating animation builder for fluent configuration.
inline auto Translating_(const Animation::Mode mode) {
    return TranslatingBuilder(mode);
}
