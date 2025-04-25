#pragma once
#include "Animation.h"
#include <GLM/glm.hpp>
class Transform;
using namespace glm;


class Rotation : public Animation {
public:
    explicit Rotation(vec3, float = 360.0f, float = 4.0f, Mode = LOOP);
    Transform GetOffset() override;

protected:
    float totalAngle;
    vec3 rotationAxis = vec3(0.0f, 1.0f, 0.0f);
};
