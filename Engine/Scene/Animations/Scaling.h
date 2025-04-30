#pragma once
#include "Animation.h"
#include <GLM/glm.hpp>
class Transform;
using namespace glm;


class Scaling final : public Animation {
public:
    explicit Scaling(Mode);
    Scaling(Mode, float, const vec3&);
    void SetTargetScale(float);
    void SetTargetScale(const vec3&);
    [[nodiscard]] const vec3& GetTargetScale() const;
    [[nodiscard]] Transform GetOffset() override;

private:
    vec3 targetScale;
};
