#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>


namespace pixelforge::scene::transform {

    /// Represents a 3D transformation composed of translation, rotation and scale,
    /// providing functionality to build, combine and convert transformations.
    class Transform final {
    public:
        /// Default constructor initializing an identity transform.
        Transform();

        /// Constructs a transform from a 4x4 transformation matrix.
        /// @param matrix The transformation matrix to decompose.
        explicit Transform(const glm::mat4& matrix);

        /// Constructs a transform from translation, quaternion rotation and scale.
        /// @param translation The translation vector.
        /// @param rotation The rotation quaternion.
        /// @param scale The scale vector.
        Transform(const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale);

        /// Constructs a transform from translation, Euler rotation and scale.
        /// @param translation The translation vector.
        /// @param eulerAngles 3D vector with Euler angles (in degrees).
        /// @param scale The scale vector.
        Transform(const glm::vec3& translation, const glm::vec3& eulerAngles, const glm::vec3& scale);

        /// Composes the transforms from a 4x4 transformation matrix.
        /// @param matrix The transformation matrix to decompose.
        void composeFromMatrix(const glm::mat4& matrix);

        /// Sets the translation of the transform using a translation vector.
        /// @param translation The translation vector.
        void setTranslation(const glm::vec3& translation);

        /// Sets the translation of the transform using individual distances.
        /// @param translateX Translation along the X-axis.
        /// @param translateY Translation along the Y-axis.
        /// @param translateZ Translation along the Z-axis.
        void setTranslation(float translateX, float translateY, float translateZ);

        /// Sets the rotation of the transform using a quaternion.
        /// @param rotation The rotation quaternion.
        void setRotation(const glm::quat& rotation);

        /// Sets the rotation of the transform using an Euler angle vector.
        /// @param eulerAngles 3D vector with Euler angles (in degrees).
        void setRotation(const glm::vec3& eulerAngles);

        /// Sets the rotation of the transform using individual Euler angles (in degree).
        /// @param rotationX Rotation angle around X-axis (pitch).
        /// @param rotationY Rotation angle around Y-axis (yaw).
        /// @param rotationZ Rotation angle around Z-axis (roll).
        void setRotation(float rotationX, float rotationY, float rotationZ);

        /// @brief Sets the scale of the transform using a scale vector.
        /// @param scale The scale vector.
        void setScale(const glm::vec3& scale);

        /// Sets the scale of the transform uniformly in all directions.
        /// @param scale The uniform scale factor.
        void setScale(float scale);

        /// Sets the scale of the transform using individual scale factors.
        /// @param scaleX Scale factor along X-axis.
        /// @param scaleY Scale factor along Y-axis.
        /// @param scaleZ Scale factor along Z-axis.
        void setScale(float scaleX, float scaleY, float scaleZ);

        /// Converts the transform to a 4x4 transformation matrix.
        /// @return Composed transformation matrix.
        [[nodiscard]] glm::mat4 toMatrix() const;

        /// Retrieves the translation component of the transform.
        /// @return Translating 3D vector.
        [[nodiscard]] const glm::vec3& getTranslation() const;

        /// Retrieves the rotation component of the transform.
        /// @return Rotating quaternion.
        [[nodiscard]] const glm::quat& getRotation() const;

        /// Retrieves the scale component of the transform.
        /// @return Scaling 3D vector.
        [[nodiscard]] const glm::vec3& getScale() const;

        /// Combines this transform with another.
        /// @param other Transform to apply from the right side.
        /// @return Resulting composed transform.
        Transform operator*(const Transform& other) const;

    private:
        glm::vec3 translation_; ///< Translation component of the transform.
        glm::quat rotation_;    ///< Rotation component of the transform.
        glm::vec3 scale_;       ///< Scale component of the transform.
    };

} // namespace pixelforge::scene::transform
