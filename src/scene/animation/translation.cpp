#include <pixelforge/scene/animation/translation.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <pixelforge/scene/utilities.hpp>


namespace pixelforge::scene::animation {

    using transform::Transform;
    using glm::vec3;


    Translation::Translation(const Mode mode):
        Animation(mode),
        direction_(vec3(0.0f, 0.0f, 1.0f)),
        totalDistance_(1.0f) {
    }


    Translation::Translation(const Mode mode, const float duration, const vec3& direction, const float distance):
        Animation(mode, duration),
        direction_() {
        this->totalDistance_ = distance;
        this->direction_ = utilities::validateDirection(direction, "Translation::Translation");
    }


    void Translation::setTotalDistance(const float distance) {
        this->totalDistance_ = distance;
    }


    void Translation::setDirection(const vec3& direction) {
        this->direction_ = utilities::validateDirection(direction, "Translation::SetDirection");
    }


    float Translation::getTotalDistance() const {
        return totalDistance_;
    }


    const vec3& Translation::getDirection() const {
        return direction_;
    }


    Transform Translation::getOffset() {
        Transform offset;
        offset.setTranslation(getProgress() * totalDistance_ * direction_);
        return offset;
    }

} // namespace pixelforge::scene::animation
