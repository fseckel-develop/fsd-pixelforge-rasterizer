#pragma once
#include "AnimationBuilder.h"
#include "../../Animations/Translating.h"


class TranslatingBuilder final : public AnimationBuilder<Translating, TranslatingBuilder> {
public:
    explicit TranslatingBuilder(const Animation::Mode mode):
        AnimationBuilder(mode) {
    }

    auto& withDirection(const vec3& direction) {
        animation->SetDirection(direction);
        return *this;
    }

    auto& withTotalDistance(const float distance) {
        animation->SetTotalDistance(distance);
        return *this;
    }
};


inline auto Translating_(const Animation::Mode mode) {
    return TranslatingBuilder(mode);
}
