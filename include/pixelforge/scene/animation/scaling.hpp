#pragma once
#include <pixelforge/scene/animation/animation.hpp>
#include <GLM/glm.hpp>

namespace pixelforge::scene::transform { class Transform; }

namespace pixelforge::scene::animation {

    /// Extension of the Animation class which scales
    /// an object to a specified target scale over time.
    class Scaling final : public Animation {
    public:
        /// Constructs a scaling animation with a specified mode and default scale (1, 1, 1).
        /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
        explicit Scaling(Mode mode);

        /// Constructs a scaling animation with a custom target scale and duration.
        /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
        /// @param duration Total duration of the animation in seconds.
        /// @param targetScale Final scale the object will reach.
        Scaling(Mode mode, float duration, const glm::vec3& targetScale);

        /// Sets a uniform scalar scale target.
        /// @param scale The target scale factor for all axes.
        void setTargetScale(float scale);

        /// Sets a non-uniform target scale vector.
        /// @param scale The 3D vector for scaling along X/Y/Z axis.
        void setTargetScale(const glm::vec3& scale);

        /// Retrieves the target scale vector.
        /// @return Reference to the scale vector.
        [[nodiscard]] const glm::vec3& getTargetScale() const;

        /// @brief Computes the current scaling transformation based on the animation progress.
        /// @return Transform containing the progress-weighted scaling.
        [[nodiscard]] transform::Transform getOffset() const override;

    private:
        glm::vec3 targetScale_; ///< The final target scale reached at 100% animation progress.
    };

} // pixelforge::scene::animation
