#include "NURBS.h"
#include <iostream>


NURBS::NURBS(const vector<vec3>& points, const vector<float>& weights, const int degree, const CurveForm form):
    BSpline(points, degree, form),
    weights(weights) {
    curveType = Curve::NURBS;
    if (curveForm == LOOP) {
        for (int i = 0; i < this->degree; i++) {
            this->weights.push_back(weights[i]);
        }
    }
    if (controlPoints.size() != weights.size()) {
        cerr << "Number of weights must match number of control points for NURBS!" << endl;
        this->weights = vector(controlPoints.size(), 1.0f);
    }
}


const vector<float>& NURBS::GetWeights() const {
    return weights;
}


vec3 NURBS::Position(const float t) const {
    return Sample(t, POSITION);
}


vec3 NURBS::Velocity(const float t) const {
    return Sample(t, VELOCITY);
}


vec3 NURBS::Acceleration(const float t) const {
    return Sample(t, ACCELERATION);
}


vec3 NURBS::Sample(const float t, const EvaluationType type) const {
    if (controlPoints.empty() || knotVector.empty()) {
        cerr << "NURBS is not initialized correctly!" << endl;
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
    vector N(degree + 1, 0.0f), dN(degree + 1, 0.0f), ddN(degree + 1, 0.0f);
    for (int i = 0; i <= degree; i++) {
        N[i] = BasisFunctionForPosition(degree, span - degree + i, tMod);
        if (type != POSITION) {
            dN[i] = BasisFunctionForVelocity(degree, span - degree + i, tMod);
        }
        if (type == ACCELERATION) {
            ddN[i] = BasisFunctionForAcceleration(degree, span - degree + i, tMod);
        }
    }
    vec3 numP(0.0f), numV(0.0f), numA(0.0f);
    float denomP = 0.0f, denomV = 0.0f, denomA = 0.0f;
    for (int i = 0; i <= degree; i++) {
        int index = (span - degree + i) % static_cast<int>(controlPoints.size());
        float weight = weights[index];
        numP += N[i] * weight * controlPoints[index];
        denomP += N[i] * weight;
        if (type != POSITION) {
            numV += dN[i] * weight * controlPoints[index];
            denomV += dN[i] * weight;
        }
        if (type == ACCELERATION) {
            numA += ddN[i] * weight * controlPoints[index];
            denomA += ddN[i] * weight;
        }
    }
    vec3 result(0.0f);
    switch (type) {
        case POSITION: result = numP / denomP; break;
        case VELOCITY: result = numV / denomP - numP * denomV / (denomP * denomP); break;
        case ACCELERATION:
            const vec3 velocity = numV / denomP - numP * denomV / (denomP * denomP);
            result = numA / denomP - 2.0f * velocity * denomV / denomP - numP * denomA / (denomP * denomP); break;
    }
    return result;
}
