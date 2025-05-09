#include "BezierCurve.h"
#include <iostream>
using namespace std; using namespace glm;


BezierCurve::BezierCurve(const vector<vec3>& points):
    Curve(points, static_cast<int>(points.size()) - 1, OPEN) {
    ComputeDerivativePoints();
}


void BezierCurve::AddControlPoint(const vec3& point) {
    Curve::AddControlPoint(point);
    ComputeDerivativePoints();
}


const vector<vec3>& BezierCurve::GetDerivativePoints(const int degree) const {
    return derivativePoints[glm::clamp(degree, 0, 1)];
}


void BezierCurve::ComputeDerivativePoints() {
    int n = degree;
    if (n < 1) return;
    derivativePoints[0].clear();
    derivativePoints[0].resize(n);
    for (int i = 0; i < n; i++) {
        derivativePoints[0][i] = static_cast<float>(n) * (controlPoints[i + 1] - controlPoints[i]);
    }
    if (--n < 1) return;
    derivativePoints[1].clear();
    derivativePoints[1].resize(n);
    for (int i = 0; i < n; i++) {
        derivativePoints[1][i] = static_cast<float>(n) * (derivativePoints[0][i + 1] - derivativePoints[0][i]);
    }
}



vec3 BezierCurve::Position(const float t) const {
    if (controlPoints.size() < 2) {
        cerr << "Not enough data to evaluate Bezier curve." << endl;
        return vec3(0.0f);
    }
    std::vector<vec3> temp = controlPoints;
    for (int r = 1; r <= degree; r++) {
        for (int i = 0; i <= degree - r; i++) {
            temp[i] = (1.0f - t) * temp[i] + t * temp[i + 1];
        }
    }
    return temp[0];
}


vec3 BezierCurve::Velocity(const float t) const {
    if (derivativePoints[0].size() < 2) {
        cerr << "Not enough data for second Bezier curve derivative." << endl;
        return vec3(0.0f);
    }
    std::vector<vec3> temp = derivativePoints[0];
    const int n = degree - 1;
    for (int r = 1; r <= n; r++) {
        for (int i = 0; i <= n - r; i++) {
            temp[i] = (1.0f - t) * temp[i] + t * temp[i + 1];
        }
    }
    return temp[0];
}


vec3 BezierCurve::Acceleration(const float t) const {
    if (derivativePoints[1].size() < 2) {
        cerr << "Not enough data for first Bezier curve derivative." << endl;
        return vec3(0.0f);
    }
    vector<vec3> temp = derivativePoints[1];
    const int n = degree - 2;
    for (int r = 1; r <= n; r++) {
        for (int i = 0; i <= n - r; i++) {
            temp[i] = (1.0f - t) * temp[i] + t * temp[i + 1];
        }
    }
    return temp[0];
}
