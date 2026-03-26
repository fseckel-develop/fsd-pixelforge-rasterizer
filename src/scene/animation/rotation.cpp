#include <pixelforge/scene/animation/rotation.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <pixelforge/scene/utilities.hpp>


namespace pixelforge::scene::animation {

    using transform::Transform;
    using glm::vec3;
    using glm::mat4;
    using glm::radians;


    Rotation::Rotation(const Mode mode):
        Animation(mode),
        rotationAxis_({0.0f, 1.0f, 0.0f}),
        totalAngle_(radians(360.0f)) {
    }


    Rotation::Rotation(const Mode mode, const float duration, const vec3& axis, const float angle):
        Animation(mode, duration),
        rotationAxis_() {
        this->rotationAxis_ = utilities::validateDirection(axis, "Rotation::Rotation");
        this->totalAngle_ = radians(angle);
    }


    void Rotation::setRotationAxis(const vec3& axis) {
        this->rotationAxis_ = utilities::validateDirection(axis, "Rotation::SetRotationAxis");
    }


    void Rotation::setTotalAngle(const float angle) {
        this->totalAngle_ = radians(angle);
    }


    const vec3& Rotation::getRotationAxis() const {
        return rotationAxis_;
    }


    float Rotation::getTotalAngle() const {
        return glm::degrees(totalAngle_);
    }


    Transform Rotation::getOffset() const {
        Transform offset;
        const float angle = totalAngle_ * getProgress();
        const mat4 rotationMatrix = rotate(mat4(1.0f), angle, rotationAxis_);
        offset.setRotation(quat_cast(rotationMatrix));
        return offset;
    }

} // namespace pixelforge::scene::animation
