#pragma once
#include "Curve.h"
#include <vector>
#include <GLM/glm.hpp>
using namespace std; using namespace glm;


class BezierCurve final : public Curve {
public:
    explicit BezierCurve(const vector<vec3>&);
    void AddControlPoint(const vec3&) override;
    [[nodiscard]] const vector<vec3>& GetDerivativePoints(int) const;

private:
    vector<vec3> derivativePoints[2];
    void ComputeDerivativePoints();
    [[nodiscard]] vec3 Position(float) const override;
    [[nodiscard]] vec3 Velocity(float t) const override;
    [[nodiscard]] vec3 Acceleration(float t) const override;
};
