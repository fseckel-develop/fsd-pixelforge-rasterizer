#include "LagrangeCurve.h"
#include <iostream>


LagrangeCurve::LagrangeCurve(const std::vector<vec3>& points):
    Curve(LAGRANGE, points, static_cast<int>(points.size()) - 1, OPEN) {
}


float LagrangeCurve::BasisFunctionForPosition(const float t, const int i) const {
    const int n = static_cast<int>(controlPoints.size());
    float result = 1.0f;
    for (int j = 0; j < n; j++) {
        if (i != j) {
            const float ti = static_cast<float>(i) / static_cast<float>(n - 1);
            const float tj = static_cast<float>(j) / static_cast<float>(n - 1);
            result *= (t - tj) / (ti - tj);
        }
    }
    return result;
}


float LagrangeCurve::BasisFunctionForVelocity(const float t, const int i) const {
    const int n = static_cast<int>(controlPoints.size());
    float result = 0.0f;
    for (int k = 0; k < n; k++) {
        if (i == k) continue;
        const float term = 1.0f / (static_cast<float>(i) / static_cast<float>(n - 1) - static_cast<float>(k) / static_cast<float>(n - 1));
        float product = 1.0f;
        for (int j = 0; j < n; j++) {
            if (j == i || j == k) continue;
            const float tj = static_cast<float>(j) / static_cast<float>(n - 1);
            const float ti = static_cast<float>(i) / static_cast<float>(n - 1);
            product *= (t - tj) / (ti - tj);
        }

        result += term * product;
    }
    return result;
}


float LagrangeCurve::BasisFunctionForAcceleration(const float t, const int i) const {
    const int n = static_cast<int>(controlPoints.size());
    float result = 0.0f;
    for (int k = 0; k < n; k++) {
        if (i == k) continue;
        for (int m = 0; m < n; m++) {
            if (m == i || m == k) continue;
            const float term = 1.0f /
                ((static_cast<float>(i) / static_cast<float>(n - 1) - static_cast<float>(k) / static_cast<float>(n - 1)) *
                (static_cast<float>(i) / static_cast<float>(n - 1) - static_cast<float>(m) / static_cast<float>(n - 1)));
            float product = 1.0f;
            for (int j = 0; j < n; j++) {
                if (j == i || j == k || j == m) continue;
                const float tj = static_cast<float>(j) / static_cast<float>(n - 1);
                const float ti = static_cast<float>(i) / static_cast<float>(n - 1);
                product *= (t - tj) / (ti - tj);
            }
            result += term * product;
        }
    }
    return result;
}


vec3 LagrangeCurve::Position(const float t) const {
    return Sample(t, POSITION);
}


vec3 LagrangeCurve::Velocity(float t) const {
    return Sample(t, VELOCITY);
}


vec3 LagrangeCurve::Acceleration(const float t) const {
    return Sample(t, ACCELERATION);
}


vec3 LagrangeCurve::Sample(const float t, const EvaluationType type) const {
    if (controlPoints.size() < 2) {
        cerr << "Not enough data to evaluate Lagrange curve." << endl;
        return vec3(0.0f);
    }
    vec3 result(0.0f);
    for (int i = 0; i < static_cast<int>(controlPoints.size()); ++i) {
        switch (type) {
            case POSITION: result += BasisFunctionForPosition(t, i) * controlPoints[i]; break;
            case VELOCITY: result += BasisFunctionForVelocity(t, i) * controlPoints[i]; break;
            case ACCELERATION: result += BasisFunctionForAcceleration(t, i) * controlPoints[i]; break;
        }
    }
    return result;
}
