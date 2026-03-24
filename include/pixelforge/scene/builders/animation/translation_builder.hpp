#pragma once
#include <pixelforge/scene/builders/animation/animation_builder.hpp>
#include <pixelforge/scene/animation/translation.hpp>


namespace pixelforge::scene::animation {

    /// Extension of the AnimationBuilder class for building
    ///translating animations using a fluent interface.
    class TranslationBuilder final : public AnimationBuilder<Translation, TranslationBuilder> {
    public:
        /// Starts the building process for a translating animation in the given mode.
        /// @param mode The animation mode to use (LOOP, ONCE, BOUNCE).
        explicit TranslationBuilder(const Animation::Mode mode):
            AnimationBuilder(mode) {
        }

        /// Sets the direction of animated translation.
        /// @param direction 3D vector direction to move along.
        /// @return Reference to this builder for method chaining.
        auto& withDirection(const glm::vec3& direction) {
            animation_->setDirection(direction);
            return *this;
        }

        /// Sets the total distance to translate over the animated duration.
        /// @param distance Total distance in world units.
        /// @return Reference to this builder for method chaining.
        auto& withTotalDistance(const float distance) {
            animation_->setTotalDistance(distance);
            return *this;
        }
    };


    /// Alias function to start building a translation animation.
    /// @param mode The animation mode to use (LOOP, ONCE, BOUNCE).
    /// @return Translating animation builder for fluent configuration.
    inline auto Translation_(const Animation::Mode mode) {
        return TranslationBuilder(mode);
    }

} // namespace pixelforge::scene::animation
