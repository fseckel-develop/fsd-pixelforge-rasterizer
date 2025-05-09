#pragma once
#include "Rotating.h"
#include <GLM/glm.hpp>
class Transform;


/// Extension of the Rotating class which moves an object along an
/// orbit, combining rotation and translation in a circular path.
class Orbiting final : public Rotating {
public:
    /// Constructs an Orbiting animation with a specified mode.
    /// @param mode The playback mode (ONCE, LOOP, BOUNCE).

    explicit Orbiting(Mode mode);
    /// Constructs an Orbiting animation with custom duration, radius, axis, and angle.
    /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
    /// @param duration Duration in seconds of a full orbit.
    /// @param radius Radius of the circular orbit.
    /// @param axis Axis around which the orbit occurs.
    /// @param angle Angle of revolution over time in degrees (default: 360).

    Orbiting(Mode mode, float duration, float radius, const glm::vec3& axis, float angle = 360.0f);
    /// Sets the radius of the orbital path.
    /// @param radius Distance from the orbit center.
    void SetRadius(float radius);

    /// Gets the radius of the orbital path.
    /// @return The current orbital radius.
    [[nodiscard]] float GetRadius() const;

    /// Computes the current transform combining orbital translation and rotation.
    /// @return Transform containing both translation along the orbit and corresponding rotation.
    [[nodiscard]] Transform GetOffset() override;

private:
    float radius; ///< Radius of the circular orbit.
};
