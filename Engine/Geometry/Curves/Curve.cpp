#include "Curve.h"


Curve::Curve(const CurveType type, const vector<vec3>& points, const int degree, const CurveForm form):
    curveType(type),
    curveForm(form),
    degree(degree),
    controlPoints(points) {
}


float Curve::EstimateParameter(const vec3& point, const int maxIterations, const float tolerance) const {
    float bestT = 0.0f;
    float bestDist = numeric_limits<float>::max();
    for (int i = 0; i <= 10; i++) {
        const float t = static_cast<float>(i) / 10;
        vec3 samplePoint = Position(t);
        const float dist = length(samplePoint - point);
        if (dist < bestDist) {
            bestDist = dist;
            bestT = t;
        }
    }
    for (int i = 0; i < maxIterations; i++) {
        vec3 curvePoint = Position(bestT);
        vec3 tangent = Velocity(bestT);
        vec3 errorVector = curvePoint - point;
        const float error = length(errorVector);
        if (error < tolerance) break;
        const float num = dot(errorVector, tangent);
        const float denom = dot(tangent, tangent);
        if (abs(denom) < 1e-6) break;
        const float deltaT = num / denom;
        bestT -= deltaT;
        bestT = glm::clamp(bestT, 0.0f, 1.0f);
    }
    return bestT;
}


void Curve::SetParameterRange(const float tMin, const float tMax) {
    if (tMin < tMax) {
        this->tMin = tMin;
        this->tMax = tMax;
    }
    else {
        this->tMin = tMax;
        this->tMax = tMin;
    }
}


void Curve::SetParameterRange(const vec3& start, const vec3& end) {
    const float tStart = EstimateParameter(start);
    const float tEnd = EstimateParameter(end);
    SetParameterRange(tStart, tEnd);
}


void Curve::AddControlPoint(const vec3& point) {
    controlPoints.push_back(point);
    degree += 1;
}


Curve::CurveForm Curve::GetForm() const {
    return curveForm;
}


float Curve::GetTMin() const {
    return tMin;
}


float Curve::GetTMax() const {
    return tMax;
}


vec3 Curve::GetCentroid() const {
    return centroid;
}


vec3 Curve::Evaluate(float t, const EvaluationType target) const {
    switch (curveForm) {
        case OPEN: t = glm::clamp(t, tMin, tMax); break;
        case LOOP: t = fmod(t - tMin, tMax - tMin) + tMin; break;
    }
    switch (target) {
        case POSITION: return Position(t);
        case VELOCITY: return Velocity(t);
        case ACCELERATION: return Acceleration(t);
        default: return vec3(0.0f);
    }
}


std::vector<vec3> Curve::Sample(const uint resolution, const EvaluationType type) const {
    std::vector<vec3> sampledPoints;
    const float step = (tMax - tMin) / static_cast<float>(resolution);
    float t = tMin;
    while (t <= tMax) {
        switch (type) {
            case POSITION: sampledPoints.push_back(Position(t)); break;
            case VELOCITY: sampledPoints.push_back(Velocity(t)); break;
            case ACCELERATION: sampledPoints.push_back(Acceleration(t)); break;
            default: break;
        }
        t += step;
    }
    return sampledPoints;
}


// TODO: ComputeCurveLength -> Evaluation resolution dependent on length?

// TODO: GetSegmentLength


void Curve::ComputeCentroid() {
    centroid = vec3(0.0f);
    const vector<vec3> positions = Sample();
    const vector<vec3> velocities = Sample();
    if (positions.size() < 2 || velocities.size() < 2) {
        return;
    }
    float totalLength = 0.0f;
    for (size_t i = 0; i < positions.size() - 1; ++i) {
        const float v1 = length(velocities[i]);
        const float v2 = length(velocities[i + 1]);
        const float segmentLength = 0.5f * (v1 + v2) * ((tMax - tMin) / static_cast<float>(300));
        totalLength += segmentLength;
        centroid += positions[i] * segmentLength;
    }
    centroid = totalLength > 0
        ? centroid / totalLength
        : vec3(0.0f);
}


mat3 Curve::ComputeFrenetFrame(const float t) const {
    vec3 tangent = Evaluate(t, VELOCITY);
    if (length(tangent) < 1e-6f) {
        return lastFrenetFrame;
    }
    tangent = normalize(tangent);
    vec3 normal = Evaluate(t, ACCELERATION);
    if (length(normal) < 1e-6f) {
        normal = abs(dot(tangent, vec3(0, 1, 0))) < 0.9f
                 ? vec3(0, 1, 0)
                 : vec3(1, 0, 0);
    } else {
        normal = normalize(normal);
    }
    normal = normalize(normal - dot(normal, tangent) * tangent);
    vec3 binormal = normalize(cross(tangent, normal));
    normal = normalize(cross(binormal, tangent));
    const bool tangentChanged = length(cross(lastFrenetFrame[0], tangent)) > 0.01f;
    const bool normalChanged = length(cross(lastFrenetFrame[1], normal)) > 0.01f;
    if (tangentChanged || normalChanged) {
        lastFrenetFrame = {tangent, normal, binormal};
    }
    return lastFrenetFrame;
}


mat3 Curve::ComputeRotMinFrame(const float t) const {
    vec3 tangent = Evaluate(t, VELOCITY);
    if (length(tangent) < 1e-6f) {
        return lastRotMinFrame;
    }
    tangent = normalize(tangent);
    const vec3 v = cross(lastRotMinFrame[0], tangent);
    const float sinTheta = length(v);
    const float cosTheta = dot(lastRotMinFrame[0], tangent);
    vec3 normal;
    if (sinTheta > 1e-6f) {
        normal = cosTheta * lastRotMinFrame[1] + sinTheta * cross(normalize(v), lastRotMinFrame[1]);
        normal = normalize(normal);
    } else {
        normal = lastRotMinFrame[1];
    }
    const vec3 binormal = normalize(cross(tangent, normal));
    lastRotMinFrame = mat3(tangent, normal, binormal);
    return lastRotMinFrame;
}


mat3 Curve::ComputeCentroidFrame(const float t) const {
    vec3 tangent = Evaluate(t, VELOCITY);
    if (length(tangent) < 1e-6f) {
        return lastCentroidFrame;
    }
    tangent = normalize(tangent);
    vec3 normal = centroid - Evaluate(t, POSITION);
    if (length(normal) < 1e-6f) {
        normal = abs(dot(tangent, vec3(0, 1, 0))) < 0.9f
                 ? vec3(0, 1, 0)
                 : vec3(1, 0, 0);
    } else {
        normal = normalize(normal);
    }
    vec3 binormal = normalize(cross(tangent, normal));
    normal = normalize(cross(binormal, tangent));
    const bool tangentChanged = length(cross(lastCentroidFrame[0], tangent)) > 0.01f;
    const bool normalChanged = length(cross(lastCentroidFrame[1], normal)) > 0.01f;
    if (tangentChanged || normalChanged) {
        lastCentroidFrame = {tangent, normal, binormal};
    }
    return lastCentroidFrame;
}
