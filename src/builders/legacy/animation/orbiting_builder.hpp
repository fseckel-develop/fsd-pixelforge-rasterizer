#pragma once
#include "animation_builder.hpp"
#include <pixelforge/scene/animation/orbiting.hpp>


namespace pixelforge::scene::animation {

    /// Extension of the AnimationBuilder class for building
    /// orbiting animations using a fluent interface.
    class OrbitingBuilder final : public AnimationBuilder<Orbiting, OrbitingBuilder> {
    public:
        /// Starts the building process for an orbiting animation in the given mode.
        /// @param mode The animation mode to use (LOOP, ONCE, BOUNCE).
        explicit OrbitingBuilder(const Animation::Mode mode):
            AnimationBuilder(mode) {
        }

        /// Sets the rotation axis for the orbit.
        /// @param axis A normalized vector representing the rotation axis.
        /// @return Reference to this builder for method chaining.
        auto& withRotationAxis(const glm::vec3& axis) {
            animation_->setRotationAxis(axis);
            return *this;
        }

        /// Sets the total angle (in degrees) to be covered by the orbit.
        /// @param angle The total orbital angle in degrees.
        /// @return Reference to this builder for method chaining.
        auto& withTotalAngle(const float angle) {
            animation_->setTotalAngle(angle);
            return *this;
        }

        /// Sets the radius of the orbit.
        /// @param radius The distance from the orbit center.
        /// @return Reference to this builder for method chaining.
        auto& withRadius(const float radius) {
            animation_->setRadius(radius);
            return *this;
        }
    };


    /// Alias function to begin building an Orbiting animation.
    /// @param mode The animation mode to use (LOOP, ONCE, BOUNCE).
    /// @return Orbiting animation builder for fluent configuration.
    inline auto Orbiting_(const Animation::Mode mode) {
        return OrbitingBuilder(mode);
    }

} // namespace pixelforge::scene::animation
