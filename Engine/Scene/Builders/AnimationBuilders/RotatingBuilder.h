#pragma once
#include "AnimationBuilder.h"
#include "../../Animations/Rotating.h"


class RotatingBuilder final : public AnimationBuilder<Rotating, RotatingBuilder> {
public:
    explicit RotatingBuilder(const Animation::Mode mode):
        AnimationBuilder(mode) {
    }

    auto& withRotationAxis(const vec3& axis) {
        animation->SetRotationAxis(axis);
        return *this;
    }

    auto& withTotalAngle(const float angle) {
        animation->SetTotalAngle(angle);
        return *this;
    }
};


inline auto Rotating_(const Animation::Mode mode) {
    return RotatingBuilder(mode);
}
