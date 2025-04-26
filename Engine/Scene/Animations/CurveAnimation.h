#pragma once
#include "Animation.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <memory>
class Curve;
using namespace std; using namespace glm;


class CurveAnimation final : public Animation {
public:
    explicit CurveAnimation(Curve*, float = 4.0, Mode = BOUNCE);
    [[nodiscard]] shared_ptr<Curve> GetCurve() const;
    [[nodiscard]] Transform GetOffset() override;

private:
    shared_ptr<Curve> curve;
    quat lastRotation = quat(1.0f, 0.0f, 0.0f, 0.0f);
    vec3 lastTangent = vec3(1.0f, 0.0f, 0.0f);
    vec3 lastNormal = vec3(0.0f, 1.0f, 0.0f);
};
