#pragma once
#include "AnimationBuilder.h"
#include "../../Animations/Orbiting.h"


class OrbitingBuilder final : public AnimationBuilder<Orbiting, OrbitingBuilder> {
public:
    explicit OrbitingBuilder(const Animation::Mode mode):
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

    auto& withRadius(const float radius) {
        animation->SetRadius(radius);
        return *this;
    }
};


inline auto Orbiting_(const Animation::Mode mode) {
    return OrbitingBuilder(mode);
}
