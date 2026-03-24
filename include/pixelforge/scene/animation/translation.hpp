#pragma once
#include <pixelforge/scene/animation/animation.hpp>
#include <GLM/glm.hpp>

namespace pixelforge::scene::transform { class Transform; }

namespace pixelforge::scene::animation {

    /// Extension of the Animation class which translates
    /// an object around a specified axis over time.
    class Translation final : public Animation {
    public:
        /// Constructs a translating animation with a specified mode,
        /// default distance of 1.0 and default direction (0, 0, 1).
        /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
        explicit Translation(Mode mode);

        /// Constructs a translating animation with custom values.
        /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
        /// @param duration Total duration of the animation in seconds.
        /// @param direction Unit vector representing translation direction.
        /// @param distance Total distance to translate over the animation's duration.
        Translation(Mode mode, float duration, const glm::vec3& direction, float distance);

        /// Sets the total distance to translate.
        /// @param distance The full movement distance.
        void setTotalDistance(float distance);

        /// Sets the direction of translation.
        /// @param direction 3D vector direction to move along.
        void setDirection(const glm::vec3& direction);

        /// Gets the total distance of the translation.
        /// @return Distance scalar value.
        [[nodiscard]] float getTotalDistance() const;

        /// Gets the direction vector for the translation.
        /// @return Reference to a unit vector representing direction.
        [[nodiscard]] const glm::vec3& getDirection() const;

        /// Calculates the current translation offset based on the animation progress.
        /// @return Transform containing the progress-weighted translation.
        [[nodiscard]] transform::Transform getOffset() override;

    private:
        glm::vec3 direction_; ///< Direction in which the translation occurs.
        float totalDistance_; ///< Total distance reached at 100% animation progress.
    };

} // pixelforge::scene::animation
