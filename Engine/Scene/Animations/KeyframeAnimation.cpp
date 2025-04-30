#include "KeyframeAnimation.h"


KeyframeAnimation::KeyframeAnimation(const Mode mode):
    Animation(mode, 0.01f) {
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
    if (keyframes.empty()) return {};
    if (keyframes.size() == 1) return keyframes.front().transform;
    return Interpolate(GetProgress());
}


Transform KeyframeAnimation::Interpolate(const float progress) const {
    const float time = progress * duration;
    for (size_t i = 0; i < keyframes.size() - 1; i++) {
        const auto& [timeStamp1, transform1] = keyframes[i];
        const auto& [timeStamp2, transform2] = keyframes[i + 1]; // NOLINT
        if (timeStamp1 < time && time <= timeStamp2) {
            const float factor = (time - timeStamp1) / (timeStamp2 - timeStamp1);
            Transform result;
            result.SetTranslation(mix(transform1.GetTranslation(), transform2.GetTranslation(), factor));
            result.SetRotation(slerp(transform1.GetRotation(), transform2.GetRotation(), factor));
            result.SetScale(mix(transform1.GetScale(), transform2.GetScale(), factor));
            return result;
        }
    }
    return keyframes.back().transform;
}
