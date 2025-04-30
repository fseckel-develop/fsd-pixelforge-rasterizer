#pragma once
#include "AnimationBuilder.h"
#include "../../Animations/KeyframeAnimation.h"


class KeyframeAnimationBuilder final : public AnimationBuilder<KeyframeAnimation, KeyframeAnimationBuilder> {
public:
    explicit KeyframeAnimationBuilder(const Animation::Mode mode):
        AnimationBuilder(mode) {
    }

    auto& withKeyframe(const float timeStamp, const Transform& transform) {
        animation->AddKeyframe(timeStamp, transform);
        return *this;
    }
};


inline auto KeyframeAnimation_(const Animation::Mode mode) {
    return KeyframeAnimationBuilder(mode);
}
