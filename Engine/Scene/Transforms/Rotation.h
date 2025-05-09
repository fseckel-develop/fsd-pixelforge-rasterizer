#pragma once
#include "Transform.h"
#include <GLM/glm.hpp>


/// Represents a pure rotation as a wrapper around a transform.
class Rotation final {
public:
    /// Default constructor initializing an identity transform.
    Rotation() = default;

    /// Sets a rotation from a quaternion.
    /// @param rotation The rotation quaternion.
    explicit Rotation(const glm::quat& rotation);

    /// Constructs a rotation from an Euler angle vector.
    /// @param eulerAngles 3D vector with Euler angles (in degrees).
    explicit Rotation(const glm::vec3& eulerAngles);

    /// Constructs a rotation from individual Euler angles (in degree).
    /// @param rotationX Rotation angle around X-axis (pitch).
    /// @param rotationY Rotation angle around Y-axis (yaw).
    /// @param rotationZ Rotation angle around Z-axis (roll).
    Rotation(float rotationX, float rotationY, float rotationZ);

    /// Provides the underlying rotating transform for usage.
    /// @return Transform representing the rotation.
    operator Transform() const { return rotation; } // NOLINT

private:
    Transform rotation; ///< The underlying rotating transform.
};
