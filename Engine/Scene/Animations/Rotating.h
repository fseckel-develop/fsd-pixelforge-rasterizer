#pragma once
#include "Animation.h"
#include <GLM/glm.hpp>
class Transform;
using namespace glm;


class Rotating : public Animation {
public:
    explicit Rotating(Mode);
    Rotating(Mode, float, const vec3&, float = 360.0f);
    void SetRotationAxis(const vec3&);
    void SetTotalAngle(float);
    [[nodiscard]] const vec3& GetRotationAxis() const;
    [[nodiscard]] float GetTotalAngle() const;
    [[nodiscard]] Transform GetOffset() override;

protected:
    vec3 rotationAxis;
    float totalAngle;
};
