#pragma once
#include "animation_builder.hpp"
#include <pixelforge/scene/animation/curve_animation.hpp>


namespace pixelforge::scene::animation {

    /// Extension of the AnimationBuilder class for building
    /// curve animations using a fluent interface.
    class CurveAnimationBuilder final : public AnimationBuilder<CurveAnimation, CurveAnimationBuilder> {
    public:
        /// Starts the building process for a curve animation in the given mode.
        /// @param mode The animation mode to use (LOOP, ONCE, BOUNCE).
        explicit CurveAnimationBuilder(const Animation::Mode mode):
            AnimationBuilder(mode) {
        }

        /// Assigns the curve along which the animation moves the object.
        /// @param curve Shared pointer to a curve representing the trajectory.
        /// @return Reference to this builder for method chaining.
        auto& withCurve(const std::shared_ptr<geometry::Curve>& curve) {
            animation_->setCurve(curve);
            return *this;
        }
    };


    /// Alias function to begin building a curve animation.
    /// @param mode The animation mode to use (LOOP, ONCE, BOUNCE).
    /// @return Curve animation builder for fluent configuration.
    inline auto CurveAnimation_(const Animation::Mode mode) {
        return CurveAnimationBuilder(mode);
    }

} // namespace pixelforge::scene::animation
