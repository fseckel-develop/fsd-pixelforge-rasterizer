#pragma once
#include "Animation.h"
#include <GLM/glm.hpp>
class Transform;


/// Extension of the Animation class which rotates
/// an object around a specified axis over time.
class Rotating : public Animation {
public:
    /// Constructs a rotating animation with a specified mode and default axis (0, 1, 0).
    /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
    explicit Rotating(Mode mode);

    /// Constructs a rotating animation with custom duration, axis, and angle.
    /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
    /// @param duration Total duration in seconds.
    /// @param axis The axis to rotate around.
    /// @param angle Total angle in degrees (default: 360).
    Rotating(Mode mode, float duration, const glm::vec3& axis, float angle = 360.0f);

    /// Sets the axis around which the object will rotate.
    /// @param axis A non-zero direction vector.
    void SetRotationAxis(const glm::vec3& axis);

    /// Sets the total angle of rotation over the animation.
    /// @param angle The rotation angle in degrees.
    void SetTotalAngle(float angle);

    /// Returns the axis around which the object rotates.
    /// @return The current rotation axis.
    [[nodiscard]] const glm::vec3& GetRotationAxis() const;

    /// Returns the total angle of rotation.
    /// @return The total angle in degrees.
    [[nodiscard]] float GetTotalAngle() const;

    /// Computes the current rotation transform based on the animation progress.
    /// @return Transform containing the progress-weighted rotation.
    [[nodiscard]] Transform GetOffset() override;

protected:
    glm::vec3 rotationAxis; ///< The axis of rotation (normalized).
    float totalAngle; ///< Total rotation angle in degrees reached at 100% animation progress.
};
