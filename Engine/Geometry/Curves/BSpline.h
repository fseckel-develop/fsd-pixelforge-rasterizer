#pragma once
#include "Curve.h"
#include <vector>
#include <GLM/glm.hpp>
using namespace std; using namespace glm;


class BSpline : public Curve {
public:
    explicit BSpline(const vector<vec3>&, int = 3, CurveForm = OPEN);
    [[nodiscard]] const vector<float>& GetKnotVector() const;

protected:
    vector<float> knotVector;
    [[nodiscard]] int FindKnotSpan(float) const;
    [[nodiscard]] float BasisFunctionForPosition(int, int, float) const;
    [[nodiscard]] float BasisFunctionForVelocity(int, int, float) const;
    [[nodiscard]] float BasisFunctionForAcceleration(int, int, float) const;

private:
    void GenerateKnotVector();
    [[nodiscard]] vec3 Position(float) const override;
    [[nodiscard]] vec3 Velocity(float) const override;
    [[nodiscard]] vec3 Acceleration(float) const override;
    [[nodiscard]] virtual vec3 Sample(float, EvaluationType) const;
};
