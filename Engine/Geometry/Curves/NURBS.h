#pragma once
#include "BSpline.h"
#include <vector>
#include <GLM/glm.hpp>
using namespace std;
using namespace glm;


class NURBS final : public BSpline {
public:
    explicit NURBS(const vector<vec3>&, const vector<float>&, int, CurveForm = OPEN);

private:
    vector<float> weights;
    [[nodiscard]] vec3 Position(float) const override;
    [[nodiscard]] vec3 Velocity(float) const override;
    [[nodiscard]] vec3 Acceleration(float) const override;
    [[nodiscard]] vec3 Sample(float t, EvaluationType) const override;
};
