#include "KeyframeAnimation.h"


KeyframeAnimation::KeyframeAnimation(const Mode mode):
    Animation(0.0f, mode) {
}


void KeyframeAnimation::AddKeyframe(const float timeStamp, const Transform& transform) {
    keyframes.emplace_back(timeStamp, transform);
    ranges::sort(keyframes, [](const auto& a, const auto& b) {
        return a.timeStamp < b.timeStamp;
    });
    if (duration < timeStamp) {
        duration = timeStamp;
    }
}


const vector<Keyframe>& KeyframeAnimation::GetKeyframes() const {
    return keyframes;
}


Transform KeyframeAnimation::GetOffset() {
    return Interpolate(GetProgress());
}


Transform KeyframeAnimation::Interpolate(const float progress) const {
    if (keyframes.size() == 1) {
        return keyframes.front().transform;
    }
    const float time = progress * duration;
    for (size_t i = 0; i < keyframes.size() - 1; i++) {
        const auto& [timeStamp1, transform1] = keyframes[i];
        const auto& [timeStamp2, transform2] = keyframes[i + 1];
        if (timeStamp1 < time && time <= timeStamp2) {
            const float factor = (time - timeStamp1) / (timeStamp2 - timeStamp1);
            Transform result;
            result.translation = mix(transform1.translation, transform2.translation, factor);
            result.rotation = slerp(transform1.rotation, transform2.rotation, factor);
            result.scaling = mix(transform1.scaling, transform2.scaling, factor);
            return result;
        }
    }
    return keyframes.back().transform;
}
