#pragma once
#include "Curve.h"
#include <vector>
#include <GLM/glm.hpp>
using namespace std;
using namespace glm;


class BSpline : public Curve {
public:
    explicit BSpline(const vector<vec3>&, int = 3, CurveForm = OPEN);

protected:
    vector<float> knotVector;
    [[nodiscard]] int FindKnotSpan(float t) const;
    [[nodiscard]] float BasisFunctionForPosition(int g, int i, float t) const;
    [[nodiscard]] float BasisFunctionForVelocity(int g, int i, float t) const;
    [[nodiscard]] float BasisFunctionForAcceleration(int g, int i, float t) const;

private:
    void GenerateKnotVector();
    [[nodiscard]] vec3 Position(float) const override;
    [[nodiscard]] vec3 Velocity(float t) const override;
    [[nodiscard]] vec3 Acceleration(float t) const override;
    [[nodiscard]] virtual vec3 Sample(float t, EvaluationType) const;
};
