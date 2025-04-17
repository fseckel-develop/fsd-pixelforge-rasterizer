#pragma once
#include "Animation.h"
#include <GLM/glm.hpp>
#include <GLM/detail/type_quat.hpp>
class Curve;
using namespace glm;


class CurveAnimation final : public Animation {
public:
    CurveAnimation(Curve*, float, Mode);
    Transform GetOffset(float) override;

private:
    Curve* curve;
    quat lastRotation = quat(1.0f, 0.0f, 0.0f, 0.0f);
    vec3 lastTangent = vec3(1.0f, 0.0f, 0.0f);
    vec3 lastNormal = vec3(0.0f, 1.0f, 0.0f);
};
