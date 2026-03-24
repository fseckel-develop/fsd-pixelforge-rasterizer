#pragma once
#include <pixelforge/scene/builders/animation/animation_builder.hpp>
#include <pixelforge/scene/animation/scaling.hpp>


namespace pixelforge::scene::animation {

    /// Extension of the AnimationBuilder class for building
    /// scaling animations using a fluent interface.
    class ScalingBuilder final : public AnimationBuilder<Scaling, ScalingBuilder> {
    public:
        /// Starts the building process for a scaling animation in the given mode.
        /// @param mode The animation mode to use (LOOP, ONCE, BOUNCE).
        explicit ScalingBuilder(const Animation::Mode mode):
            AnimationBuilder(mode) {
        }

        /// Sets a uniform target scale for the animation.
        /// @param scale Uniform scale factor applied to all axes.
        /// @return Reference to this builder for method chaining.
        auto& withTargetScale(const float scale) {
            animation_->setTargetScale(scale);
            return *this;
        }

        /// Sets a non-uniform target scale for the animation.
        /// @param scale A 3D vector representing scaling along X/Y/Z.
        /// @return Reference to this builder for method chaining.
        auto& withTargetScale(const glm::vec3& scale) {
            animation_->setTargetScale(scale);
            return *this;
        }
    };


    /// Alias function to start building a scaling animation.
    /// @param mode The animation mode to use (LOOP, ONCE, BOUNCE).
    /// @return Scaling animation builder for fluent configuration.
    inline auto Scaling_(const Animation::Mode mode) {
        return ScalingBuilder(mode);
    }

} // namespace pixelforge::scene::animation
