#pragma once
#include "Rotation.h"
#include <GLM/glm.hpp>
class Transform;
using namespace glm;


class Orbiting final : public Rotation {
public:
    Orbiting(float, vec3, float = 360.0f, float = 4.0f, Mode = LOOP);
    Transform GetOffset(float) override;

private:
    float radius;
};
