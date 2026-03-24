#include <pixelforge/scene/transform/rotate.hpp>


namespace pixelforge::scene::transform {

    using glm::vec3;
    using glm::quat;


    Rotate::Rotate(const quat& rotation) {
        this->rotation_.setRotation(rotation);
    }


    Rotate::Rotate(const vec3& eulerAngles) {
        this->rotation_.setRotation(eulerAngles);
    }


    Rotate::Rotate(const float pitch, const float yaw, const float roll) {
        this->rotation_.setRotation(pitch, yaw, roll);
    }

} // namespace pixelforge::scene::transform
