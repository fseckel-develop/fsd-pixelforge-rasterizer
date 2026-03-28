#pragma once
#include <pixelforge/scene/animation/rotation.hpp>
#include <GLM/glm.hpp>

namespace pixelforge::scene::transform { class Transform; }

namespace pixelforge::scene::animation {

    /// Animation that moves an object along an elliptical orbit
    /// in a plane oriented by the given rotation axis.
    class EllipticOrbiting final : public Rotation {
    public:
        /// Constructs an elliptic orbiting animation with a specified mode and default axis (0, 1, 0).
        /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
        explicit EllipticOrbiting(Mode mode);

        /// Constructs an elliptic orbiting animation.
        /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
        /// @param duration Duration in seconds of a full orbit.
        /// @param semiMajorAxis Semi-major axis length.
        /// @param semiMinorAxis Semi-minor axis length.
        /// @param rotationAxis Axis defining the orbit plane orientation.
        /// @param totalAngle Total swept angle in degrees.
        /// @param useFocusOrigin Whether the orbit origin lies in an elliptic focus or the center.
        explicit EllipticOrbiting(
            Mode mode,
            float duration,
            float semiMajorAxis,
            float semiMinorAxis,
            glm::vec3 rotationAxis,
            float totalAngle = 360.0f,
            bool useFocusOrigin = false
        );

        /// Sets the semi-major axis of the ellipse.
        /// @param semiMajorAxis Semi-major axis length.
        void setSemiMajorAxis(float semiMajorAxis);

        /// Sets the semi-minor axis of the ellipse.
        /// @param semiMinorAxis Semi-minor axis length.
        void setSemiMinorAxis(float semiMinorAxis);

        /// Sets both ellipse radii.
        /// @param semiMajorAxis Semi-major axis length.
        /// @param semiMinorAxis Semi-minor axis length.
        void setAxes(float semiMajorAxis, float semiMinorAxis);

        /// Sets whether the orbit origin lies in an elliptic focus or the center.
        /// @param useFocusOrigin Whether to use focus origin or center origin.
        void setUseFocusOrigin(bool useFocusOrigin);

        /// Gets the semi-major axis.
        [[nodiscard]] float getSemiMajorAxis() const;

        /// Gets the semi-minor axis.
        [[nodiscard]] float getSemiMinorAxis() const;

        /// Checks if the orbit origin lies in an elliptic focus.
        [[nodiscard]] bool getUseFocusOrigin() const;

        /// Computes the current animation transform offset.
        [[nodiscard]] transform::Transform getOffset() const override;

    private:
        float semiMajorAxis_;    ///< Length of the semi-major axis.
        float semiMinorAxis_;    ///< Length of the semi-minor axis.
        bool useFocusOrigin_ = false; ///< Whether the orbits origin lies in an elliptic focus or the center.
    };

} // namespace pixelforge::scene::animation
