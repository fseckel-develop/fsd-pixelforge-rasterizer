#include <pixelforge/scene/animation/keyframe_animation.hpp>


namespace pixelforge::scene::animation {

    using transform::Transform;
    using std::vector;
    using std::ranges::sort;


    KeyframeAnimation::KeyframeAnimation(const Mode mode):
        Animation(mode, 0.01f) {
    }


    void KeyframeAnimation::addKeyframe(const float timestamp, const Transform& transform) {
        keyframes_.emplace_back(timestamp, transform);
        sort(keyframes_, [](const auto& a, const auto& b) {
            return a.timestamp < b.timestamp;
        });
        if (duration_ < timestamp) {
            duration_ = timestamp;
        }
    }


    const vector<Keyframe>& KeyframeAnimation::getKeyframes() const {
        return keyframes_;
    }


    Transform KeyframeAnimation::getOffset() const {
        if (keyframes_.empty()) return {};
        if (keyframes_.size() == 1) return keyframes_.front().transform;
        return interpolate(getProgress());
    }


    Transform KeyframeAnimation::interpolate(const float progress) const {
        const float time = progress * duration_;
        for (size_t i = 0; i < keyframes_.size() - 1; i++) {
            const auto& [timestampA, transformA] = keyframes_[i];
            const auto& [timestampB, transformB] = keyframes_[i + 1]; // NOLINT
            if (timestampA < time && time <= timestampB) {
                const float factor = (time - timestampA) / (timestampB - timestampA);
                Transform result;
                result.setTranslation(mix(transformA.getTranslation(), transformB.getTranslation(), factor));
                result.setRotation(slerp(transformA.getRotation(), transformB.getRotation(), factor));
                result.setScale(mix(transformA.getScale(), transformB.getScale(), factor));
                return result;
            }
        }
        return keyframes_.back().transform;
    }

} // namespace pixelforge::scene::animation
