#include "BSpline.h"
#include <iostream>
#include <GLM/common.hpp>


BSpline::BSpline(const vector<vec3>& points, const int degree, const CurveForm form):
    Curve(BSPLINE, points, degree, form) {
    ComputeCentroid();
    if (!controlPoints.empty() && controlPoints.size() <= static_cast<size_t>(this->degree)) {
        cerr << "Invalid degree for BSpline" << endl;
        this->degree = static_cast<int>(controlPoints.size()) - 1;
    }
    if (curveForm == LOOP) {
        for (int i = 0; i < degree; i++) {
            controlPoints.push_back(controlPoints[i]);
        }
    }
    GenerateKnotVector();
}


int BSpline::FindKnotSpan(float t) const {
    const int n = static_cast<int>(controlPoints.size()) - 1;
    if (curveForm == LOOP) {
        const float period = knotVector.back() - knotVector.front();
        t = fmod(t - knotVector.front(), period) + knotVector.front();
    }
    if (knotVector[n + 1] <= t) {
        return n;
    }
    if (t <= knotVector[degree]) {
        return degree;
    }
    for (int i = degree; i <= n; i++) {
        if (knotVector[i] <= t && t < knotVector[i + 1]) {
            return i;
        }
    }
    return n;
}


float BSpline::BasisFunctionForPosition(const int g, const int i, const float t) const {
    std::vector N(g + 1, 0.0f);
    for (int j = 0; j <= g; ++j) {
        N[j] = (knotVector[i + j] <= t && t < knotVector[i + j + 1])
            ? 1.0f
            : 0.0f;
    }
    for (int d = 1; d <= g; ++d) {
        for (int j = 0; j <= g - d; ++j) {
            const float denomA = knotVector[i + j + d] - knotVector[i + j];
            const float denomB = knotVector[i + j + d + 1] - knotVector[i + j + 1];
            const float termA = (denomA != 0.0f)
                ? ((t - knotVector[i + j]) / denomA) * N[j]
                : 0.0f;
            const float termB = (denomB != 0.0f)
                ? ((knotVector[i + j + d + 1] - t) / denomB) * N[j + 1]
                : 0.0f;
            N[j] = termA + termB;
        }
    }
    return N[0];
}


float BSpline::BasisFunctionForVelocity(const int g, const int i, const float t) const {
    const float denomA = knotVector[i + g] - knotVector[i];
    const float denomB = knotVector[i + g + 1] - knotVector[i + 1];
    const float termA = (denomA != 0.0f)
        ? static_cast<float>(g) / denomA * BasisFunctionForPosition(g - 1, i, t)
        : 0.0f;
    const float termB = (denomB != 0.0f)
        ? static_cast<float>(g) / denomB * BasisFunctionForPosition(g - 1, i + 1, t)
        : 0.0f;
    return termA - termB;
}


float BSpline::BasisFunctionForAcceleration(const int g, const int i, const float t) const {
    const float denomA = knotVector[i + g] - knotVector[i];
    const float denomB = knotVector[i + g + 1] - knotVector[i + 1];
    const float denomC = knotVector[i + g + 2] - knotVector[i + 2];
    const float termA = (denomA != 0.0f && denomB != 0.0f)
        ? static_cast<float>(g * (g - 1)) / (denomA * denomB) * BasisFunctionForPosition(g - 2, i, t)
        : 0.0f;
    const float termB = (denomB != 0.0f && denomC != 0.0f)
        ? static_cast<float>(g * (g - 1)) / (denomB * denomC) * BasisFunctionForPosition(g - 2, i + 1, t)
        : 0.0f;
    return termA - termB;
}


void BSpline::GenerateKnotVector() {
    const int K = static_cast<int>(controlPoints.size());
    knotVector.clear();
    if (curveForm == LOOP) {
        const int numKnots = K + degree + 1;
        for (int i = 0; i < numKnots; i++) {
            knotVector.push_back(static_cast<float>(i - degree));
        }
    } else {
        const int n = K - degree;
        for (int i = 0; i <= n; i++) {
            if (i == 0 || i == n) {
                for (int j = 0; j <= degree; j++) {
                    knotVector.push_back(static_cast<float>(i));
                }
            } else {
                knotVector.push_back(static_cast<float>(i));
            }
        }
    }
    const auto normalizer = static_cast<float>(K - degree);
    for (float& knot : knotVector) {
        knot = knot / normalizer;
    }
}


vec3 BSpline::Position(const float t) const {
    return Sample(t, POSITION);
}


vec3 BSpline::Velocity(const float t) const {
    return Sample(t, VELOCITY);
}


vec3 BSpline::Acceleration(const float t) const {
    return Sample(t, ACCELERATION);
}


vec3 BSpline::Sample(const float t, const EvaluationType type) const {
    if (controlPoints.empty() || knotVector.empty()) {
        std::cerr << "BSpline is not initialized correctly!" << std::endl;
        return vec3(0.0f);
    }
    float tMod = t;
    if (curveForm == LOOP) {
        const float period = knotVector.back() - knotVector.front();
        tMod = fmod(tMod - knotVector.front(), period) + knotVector.front();
    }
    else if (t == tMax) {
        return controlPoints.back();
    }
    const int span = FindKnotSpan(tMod);
    std::vector N(degree + 1, 0.0f);
    for (int i = 0; i <= degree; i++) {
        switch (type) {
            case POSITION: N[i] = BasisFunctionForPosition(degree, span - degree + i, tMod); break;
            case VELOCITY: N[i] = BasisFunctionForVelocity(degree, span - degree + i, tMod); break;
            case ACCELERATION: N[i] = BasisFunctionForAcceleration(degree, span - degree + i, tMod); break;
        }
    }
    vec3 result(0.0f);
    for (int i = 0; i <= degree; i++) {
        const int index = (span - degree + i) % static_cast<int>(controlPoints.size());
        result += N[i] * controlPoints[index];
    }
    return result;
}
