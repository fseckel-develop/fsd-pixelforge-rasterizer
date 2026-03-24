#include <pixelforge/scene/animation/scaling.hpp>
#include <pixelforge/scene/transform/transform.hpp>


namespace pixelforge::scene::animation {

    using transform::Transform;
    using glm::vec3;


    Scaling::Scaling(const Mode mode):
        Animation(mode),
        targetScale_(vec3(1.0f)) {
    }


    Scaling::Scaling(const Mode mode, const float duration, const vec3& targetScale):
        Animation(mode, duration),
        targetScale_() {
        this->targetScale_ = targetScale;
    }


    void Scaling::setTargetScale(const float scale) {
        this->targetScale_ = vec3(scale);
    }


    void Scaling::setTargetScale(const vec3& scale) {
        this->targetScale_ = scale;
    }


    const vec3& Scaling::getTargetScale() const {
        return targetScale_;
    }


    Transform Scaling::getOffset() {
        Transform offset;
        const float t = getProgress();
        offset.setScale(mix(vec3(1.0f), targetScale_, t));
        return offset;
    }

} // namespace pixelforge::scene::animation
