#pragma once
#include "Animation.h"
#include <GLM/glm.hpp>
class Transform;
using namespace glm;


class Translation final : public Animation {
public:
    Translation(float, vec3, float = 4.0f, Mode = BOUNCE);
    Transform GetOffset(float) override;

private:
    float totalDistance;
    vec3 direction = vec3(1.0f, 0.0f, 0.0f);
};
