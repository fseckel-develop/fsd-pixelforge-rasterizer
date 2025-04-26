#pragma once
#include <vector>
#include <GLM/glm.hpp>
using namespace std; using namespace glm;


class Curve {
public:
    enum CurveType { BEZIER, BSPLINE, HERMITE, LAGRANGE, NURBS, FUNCTION };
    enum CurveForm { LOOP, OPEN };
    enum EvaluationType { POSITION, VELOCITY, ACCELERATION };
    void SetParameterRange(float, float);
    void SetParameterRange(const vec3&, const vec3&);
    virtual void AddControlPoint(const vec3&);
    [[nodiscard]] float EstimateParameter(const vec3&, int = 10, float = 1e-4) const;
    [[nodiscard]] CurveType GetType() const;
    [[nodiscard]] CurveForm GetForm() const;
    [[nodiscard]] float GetTMin() const;
    [[nodiscard]] float GetTMax() const;
    [[nodiscard]] vec3 GetCentroid() const;
    [[nodiscard]] vector<vec3> GetControlPoints() const;
    [[nodiscard]] vector<vec3> Sample(uint = 300, EvaluationType = POSITION) const;
    [[nodiscard]] vec3 Evaluate(float t, EvaluationType = POSITION) const;
    [[nodiscard]] mat3 ComputeFrenetFrame(float) const;
    [[nodiscard]] mat3 ComputeRotMinFrame(float) const;
    [[nodiscard]] mat3 ComputeCentroidFrame(float) const;
    virtual ~Curve() = default;

protected:
    CurveType curveType;
    CurveForm curveForm;
    float tMin = 0.0f;
    float tMax = 1.0f;
    int degree;
    vec3 centroid = vec3(0.0f);
    vector<vec3> controlPoints;
    mutable mat3 lastFrenetFrame = mat3(1.0f);
    mutable mat3 lastRotMinFrame = mat3(1.0f);
    mutable mat3 lastCentroidFrame = mat3(1.0f);

    Curve(CurveType, const vector<vec3>&, int, CurveForm = OPEN);
    [[nodiscard]] virtual vec3 Position(float t) const = 0;
    [[nodiscard]] virtual vec3 Velocity(float t) const = 0;
    [[nodiscard]] virtual vec3 Acceleration(float t) const = 0;
    void ComputeCentroid();
};
