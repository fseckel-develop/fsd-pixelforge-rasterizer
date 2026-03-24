#pragma once
#include <pixelforge/scene/transform/transform.hpp>


namespace pixelforge::scene::transform {

    /// Builder class for creating transforms using a fluent interface.
    class TransformBuilder {
    public:
        /// Default constructor, instantiating an identity transform.
        TransformBuilder() = default;

        /// Sets the transform using a matrix.
        /// @param matrix The transformation matrix.
        /// @return Reference to the builder for fluent chaining.
        auto& fromMatrix(const glm::mat4& matrix) {
            transform_.composeFromMatrix(matrix);
            return *this;
        }

        /// Sets the translation of the transform being built.
        /// @param translation The translation vector.
        /// @return Reference to the builder for fluent chaining.
        auto& withTranslation(const glm::vec3& translation) {
            transform_.setTranslation(translation);
            return *this;
        }

        /// Sets the translation of the transform being built.
        /// @param translateX Translation distance along X-axis.
        /// @param translateY Translation distance along Y-axis.
        /// @param translateZ Translation distance along Z-axis.
        /// @return Reference to the builder for fluent chaining.
        auto& withTranslation(const float translateX, const float translateY, const float translateZ) {
            transform_.setTranslation(translateX, translateY, translateZ);
            return *this;
        }

        /// Sets the rotation of the build transform using a quaternion.
        /// @param rotation The rotation quaternion.
        /// @return Reference to the current builder for fluent chaining.
        auto& withRotation(const glm::quat& rotation) {
            transform_.setRotation(rotation);
            return *this;
        }

        /// Sets the rotation of the build transform using an Euler angle vector.
        /// @param eulerAngles 3D vector with Euler angles in degrees.
        /// @return Reference to the current builder for fluent chaining.
        auto& withRotation(const glm::vec3& eulerAngles) {
            transform_.setRotation(eulerAngles);
            return *this;
        }

        /// Sets the rotation of the build transform using individual Euler angles in degree.
        /// @param rotateX Rotation angle around X-axis (pitch).
        /// @param rotateY Rotation angle around Y-axis (yaw).
        /// @param rotateZ Rotation angle around Z-axis (roll).
        /// @return Reference to the current builder for fluent chaining.
        auto& withRotation(const float rotateX, const float rotateY, const float rotateZ) {
            transform_.setRotation(rotateX, rotateY, rotateZ);
            return *this;
        }

        /// Sets the scale of the transform using a scale vector.
        /// @param scale The scale vector.
        /// @return Reference to the current builder for fluent chaining.
        auto& withScale(const glm::vec3& scale) {
            transform_.setScale(scale);
            return *this;
        }

        /// Sets the scale of the transform uniformly in all directions.
        /// @param scale The uniform scale factor.
        /// @return Reference to the current builder for fluent chaining.
        auto& withScale(const float scale) {
            transform_.setScale(scale);
            return *this;
        }

        /// Sets the scale of the transform being built.
        /// @param scaleX Scale factor along X-axis.
        /// @param scaleY Scale factor along Y-axis.
        /// @param scaleZ Scale factor along Z-axis.
        /// @return Reference to the current builder for fluent chaining.
        auto& withScale(const float scaleX, const float scaleY, const float scaleZ) {
            transform_.setScale(scaleX, scaleY, scaleZ);
            return *this;
        }

        /// Finalizes the build process and returns the transform.
        /// @return The constructed transform.
        [[nodiscard]] Transform build() const {
            return transform_;
        }

        /// Implicit conversion to allow the builder to be used as a transform.
        /// @return The constructed transform.
        operator Transform() { // NOLINT
            return build();
        }

    private:
        Transform transform_; ///< The underlying transform being built.
    };


    /// Alias function to start building a transform.
    /// @return Transform builder for fluent configuration.
    inline auto Transform_() {
        return TransformBuilder();
    }

} // namespace pixelforge::scene::transform
