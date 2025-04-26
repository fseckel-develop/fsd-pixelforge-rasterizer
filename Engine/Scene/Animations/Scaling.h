#pragma once
#include "Animation.h"
#include <GLM/glm.hpp>
class Transform;
using namespace glm;


class Scaling final : public Animation {
public:
    explicit Scaling(vec3, float = 4.0f, Mode = BOUNCE);
    [[nodiscard]] const vec3& GetTargetScale() const;
    [[nodiscard]] Transform GetOffset() override;

private:
    vec3 targetScale = vec3(1.0f, 1.0f, 1.0f);
};
