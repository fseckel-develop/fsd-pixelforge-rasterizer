#pragma once
#include "Curve.h"
#include <vector>
#include <GLM/glm.hpp>
using namespace std; using namespace glm;


class LagrangeCurve final : public Curve {
public:
    explicit LagrangeCurve(const vector<vec3>&);

private:
    [[nodiscard]] float BasisFunctionForPosition(float, int) const;
    [[nodiscard]] float BasisFunctionForVelocity(float, int) const;
    [[nodiscard]] float BasisFunctionForAcceleration(float, int) const;
    [[nodiscard]] vec3 Position(float) const override;
    [[nodiscard]] vec3 Velocity(float) const override;
    [[nodiscard]] vec3 Acceleration(float) const override;
    [[nodiscard]] vec3 Sample(float, EvaluationType) const;
};
