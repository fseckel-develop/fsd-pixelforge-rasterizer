#pragma once
#include "Rotating.h"
#include <GLM/glm.hpp>
class Transform;
using namespace glm;


class Orbiting final : public Rotating {
public:
    explicit Orbiting(Mode);
    Orbiting(Mode, float, float, const vec3&, float = 360.0f);
    void SetRadius(float);
    [[nodiscard]] float GetRadius() const;
    [[nodiscard]] Transform GetOffset() override;

private:
    float radius;
};
