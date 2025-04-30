#pragma once
#include "AnimationBuilder.h"
#include "../../Animations/Scaling.h"


class ScalingBuilder final : public AnimationBuilder<Scaling, ScalingBuilder> {
public:
    explicit ScalingBuilder(const Animation::Mode mode):
        AnimationBuilder(mode) {
    }

    auto& withTargetScale(const float scale) {
        animation->SetTargetScale(scale);
        return *this;
    }

    auto& withTargetScale(const vec3& scale) {
        animation->SetTargetScale(scale);
        return *this;
    }
};


inline auto Scaling_(const Animation::Mode mode) {
    return ScalingBuilder(mode);
}
