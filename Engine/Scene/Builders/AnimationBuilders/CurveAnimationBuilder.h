#pragma once
#include "AnimationBuilder.h"
#include "../../Animations/CurveAnimation.h"


class CurveAnimationBuilder final : public AnimationBuilder<CurveAnimation, CurveAnimationBuilder> {
public:
    explicit CurveAnimationBuilder(const Animation::Mode mode):
        AnimationBuilder(mode) {
    }

    auto& withCurve(const shared_ptr<Curve>& curve) {
        animation->SetCurve(curve);
        return *this;
    }
};


inline auto CurveAnimation_(const Animation::Mode mode) {
    return CurveAnimationBuilder(mode);
}
