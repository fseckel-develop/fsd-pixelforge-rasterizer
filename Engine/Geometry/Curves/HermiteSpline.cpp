#include "HermiteSpline.h"
#include <iostream>


HermiteSpline::HermiteSpline(const std::vector<vec3>& points, std::vector<vec3>& tangents, const CurveForm form):
    Curve(HERMITE, points, static_cast<int>(points.size()) - 1, form),
    tangents(std::move(tangents)) {
    ComputeCentroid();
    if (controlPoints.size() != tangents.size()) {
        cerr << "Number of tangents must match number of control points for Hermite Spline!" << endl;
        ComputeCatmullRomTangents();
    }
}


HermiteSpline::HermiteSpline(const std::vector<vec3>& points, const CurveForm form):
    Curve(HERMITE, points, static_cast<int>(points.size()) - 1, form),
    tangents({}) {
    ComputeCatmullRomTangents();
}


void HermiteSpline::AddControlPoint(const vec3& point) {
    Curve::AddControlPoint(point);
    ComputeCatmullRomTangents();
}


void HermiteSpline::AddControlPointWithTangent(const vec3& point, const vec3& tangent) {
    Curve::AddControlPoint(point);
    tangents.push_back(tangent);
}


void HermiteSpline::SetTangents(const std::vector<vec3>& tangents) {
    this->tangents = tangents;
}


const std::vector<vec3>& HermiteSpline::GetTangents() const {
    return tangents;
}


void HermiteSpline::ComputeCatmullRomTangents() {
    const int n = static_cast<int>(controlPoints.size());
    if (n < 2) return;
    tangents.resize(n);
    for (int i = 0; i < n; i++) {
        const int prev = (i - 1 + n) % n;
        const int next = (i + 1) % n;
        tangents[i] = 0.5f * (controlPoints[next] - controlPoints[prev]);
    }
}


vec3 HermiteSpline::Position(const float t) const {
    return Sample(t, POSITION);
}


vec3 HermiteSpline::Velocity(const float t) const {
    return Sample(t, VELOCITY);
}


vec3 HermiteSpline::Acceleration(const float t) const {
    return Sample(t, ACCELERATION);
}


static float h00(const float t) { return (1.0f + 2.0f * t) * (1.0f - t) * (1.0f - t); }
static float h10(const float t) { return t * (1.0f - t) * (1.0f - t); }
static float h01(const float t) { return t * t * (3.0f - 2.0f * t); }
static float h11(const float t) { return t * t * (t - 1.0f); }

static float dh00(const float t) { return 6.0f * t * (t - 1.0f); }
static float dh10(const float t) { return (3.0f * t - 4.0f) * t + 1.0f; }
static float dh01(const float t) { return 6.0f * t * (1.0f - t); }
static float dh11(const float t) { return 3.0f * t * t - 2.0f * t; }

static float d2h00(const float t) { return 12.0f * t - 6.0f; }
static float d2h10(const float t) { return 6.0f * t - 4.0f; }
static float d2h01(const float t) { return -12.0f * t + 6.0f; }
static float d2h11(const float t) { return 6.0f * t - 2.0f; }


vec3 HermiteSpline::Sample(const float t, const EvaluationType type) const {
    if (controlPoints.size() < 2 || tangents.size() < 2) {
        cerr << "Not enough data to evaluate Hermite spline." << endl;
        return vec3(0.0f);
    }
    const int pointCount = static_cast<int>(controlPoints.size());
    const float segmentF = t * (static_cast<float>(pointCount) - (curveForm == LOOP ? 0.0f : 1.0f));
    const int segment = static_cast<int>(segmentF);
    const float localT = segmentF - static_cast<float>(segment);
    const int i0 = segment % pointCount;
    const int i1 = (segment + 1) % pointCount;
    const vec3 p0 = controlPoints[i0];
    const vec3 p1 = controlPoints[i1];
    const vec3 m0 = tangents[i0];
    const vec3 m1 = tangents[i1];
    switch (type) {
        case POSITION: return h00(localT) * p0 + h10(localT) * m0 + h01(localT) * p1 + h11(localT) * m1;
        case VELOCITY: return dh00(localT) * p0 + dh10(localT) * m0 + dh01(localT) * p1 + dh11(localT) * m1;
        case ACCELERATION: return d2h00(localT) * p0 + d2h10(localT) * m0 + d2h01(localT) * p1 + d2h11(localT) * m1;
        default: return vec3(0.0f);
    }
}
