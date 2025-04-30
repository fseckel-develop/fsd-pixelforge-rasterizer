#pragma once
#include "Animation.h"
#include <GLM/glm.hpp>
class Transform;
using namespace glm;


class Translating final : public Animation {
public:
    explicit Translating(Mode);
    Translating(Mode, float, const vec3&, float);
    void SetTotalDistance(float);
    void SetDirection(const vec3&);
    [[nodiscard]] float GetTotalDistance() const;
    [[nodiscard]] const vec3& GetDirection() const;
    [[nodiscard]] Transform GetOffset() override;

private:
    float totalDistance;
    vec3 direction;
};
