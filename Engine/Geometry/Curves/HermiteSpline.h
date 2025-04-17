#pragma once
#include "Curve.h"
#include <vector>
#include <GLM/glm.hpp>
using namespace std;
using namespace glm;


class HermiteSpline final : public Curve {
public:
    explicit HermiteSpline(const vector<vec3>&, vector<vec3>&, CurveForm = OPEN);
    explicit HermiteSpline(const vector<vec3>&, CurveForm = OPEN);
    void AddControlPoint(const vec3&) override;
    void AddControlPointWithTangent(const vec3&, const vec3&);
    void SetTangents(const vector<vec3>&);
    [[nodiscard]] const vector<vec3>& GetTangents() const;

private:
    vector<vec3> tangents;
    void ComputeCatmullRomTangents();
    [[nodiscard]] vec3 Position(float) const override;
    [[nodiscard]] vec3 Velocity(float) const override;
    [[nodiscard]] vec3 Acceleration(float) const override;
    [[nodiscard]] vec3 Sample(float, EvaluationType) const;
};
