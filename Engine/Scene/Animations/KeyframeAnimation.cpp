#include "KeyframeAnimation.h"
using namespace std;


KeyframeAnimation::KeyframeAnimation(const Mode mode):
    Animation(mode, 0.01f) {
}


void KeyframeAnimation::AddKeyframe(const float timestamp, const Transform& transform) {
    keyframes.emplace_back(timestamp, transform);
    ranges::sort(keyframes, [](const auto& a, const auto& b) {
        return a.timestamp < b.timestamp;
    });
    if (duration < timestamp) {
        duration = timestamp;
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
        const auto& [timestampA, transformA] = keyframes[i];
        const auto& [timestampB, transformB] = keyframes[i + 1]; // NOLINT
        if (timestampA < time && time <= timestampB) {
            const float factor = (time - timestampA) / (timestampB - timestampA);
            Transform result;
            result.SetTranslation(mix(transformA.GetTranslation(), transformB.GetTranslation(), factor));
            result.SetRotation(slerp(transformA.GetRotation(), transformB.GetRotation(), factor));
            result.SetScale(mix(transformA.GetScale(), transformB.GetScale(), factor));
            return result;
        }
    }
    return keyframes.back().transform;
}
