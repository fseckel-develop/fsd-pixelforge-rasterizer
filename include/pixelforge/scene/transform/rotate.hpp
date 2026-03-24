#pragma once
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>


namespace pixelforge::scene::transform {

    /// Represents a pure rotation as a wrapper around a transform.
    class Rotate final {
    public:
        /// Default constructor initializing an identity transform.
        Rotate() = default;

        /// Sets a rotation from a quaternion.
        /// @param rotation The rotation quaternion.
        explicit Rotate(const glm::quat& rotation);

        /// Constructs a rotation from an Euler angle vector.
        /// @param eulerAngles 3D vector with Euler angles (in degrees).
        explicit Rotate(const glm::vec3& eulerAngles);

        /// Constructs a rotation from individual Euler angles (in degree).
        /// @param rotationX Rotation angle around X-axis (pitch).
        /// @param rotationY Rotation angle around Y-axis (yaw).
        /// @param rotationZ Rotation angle around Z-axis (roll).
        Rotate(float rotationX, float rotationY, float rotationZ);

        /// Provides the underlying rotating transform for usage.
        /// @return Transform representing the rotation.
        operator Transform() const { return rotation_; } // NOLINT

    private:
        Transform rotation_; ///< The underlying rotating transform.
    };

} // namespace pixelforge::scene::transform
