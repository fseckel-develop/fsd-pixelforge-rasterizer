#include <pixelforge/geometry/curves/curve.hpp>


namespace pixelforge::geometry {

    using std::vector;
    using std::numeric_limits;
    using glm::vec3;
    using glm::mat3;


    Curve::Curve(const vector<vec3>& points, const int degree, const CurveForm form):
        curveForm_(form),
        degree_(degree),
        controlPoints_(points) {
    }


    void Curve::setParameterRange(const float min, const float max) {
        if (min < max) {
            this->tMin_ = min;
            this->tMax_ = max;
        } else {
            this->tMin_ = max;
            this->tMax_ = min;
        }
    }


    void Curve::setParameterRange(const vec3& pointA, const vec3& pointB) {
        const float tStart = estimateParameter(pointA);
        const float tEnd = estimateParameter(pointB);
        setParameterRange(tStart, tEnd);
    }


    void Curve::addControlPoint(const vec3& point) {
        controlPoints_.push_back(point);
        degree_ += 1;
    }


    float Curve::estimateParameter(const vec3& point, const int maxIterations, const float tolerance) const {
        float bestT = 0.0f;
        float bestDist = numeric_limits<float>::max();
        for (int i = 0; i <= 10; i++) {
            const float t = static_cast<float>(i) / 10;
            vec3 samplePoint = position(t);
            const float dist = length(samplePoint - point);
            if (dist < bestDist) {
                bestDist = dist;
                bestT = t;
            }
        }
        for (int i = 0; i < maxIterations; i++) {
            vec3 curvePoint = position(bestT);
            vec3 tangent = velocity(bestT);
            vec3 errorVector = curvePoint - point;
            if (length(errorVector) < tolerance) break;
            const float num = dot(errorVector, tangent);
            const float denom = dot(tangent, tangent);
            if (abs(denom) < 1e-6) break;
            const float deltaT = num / denom;
            bestT -= deltaT;
            bestT = glm::clamp(bestT, 0.0f, 1.0f);
        }
        return bestT;
    }


    Curve::CurveForm Curve::getForm() const {
        return curveForm_;
    }


    float Curve::getTMin() const {
        return tMin_;
    }


    float Curve::getTMax() const {
        return tMax_;
    }


    vec3 Curve::getCentroid() const {
        return centroid_;
    }


    vector<vec3> Curve::getControlPoints() const {
        return controlPoints_;
    }


    vector<vec3> Curve::sample(const uint sampleCount, const EvaluationType type) const {
        vector<vec3> sampledPoints;
        const float step = (tMax_ - tMin_) / static_cast<float>(sampleCount);
        float t = tMin_;
        while (t <= tMax_) {
            switch (type) {
                case POSITION: sampledPoints.push_back(position(t)); break;
                case VELOCITY: sampledPoints.push_back(velocity(t)); break;
                case ACCELERATION: sampledPoints.push_back(acceleration(t)); break;
                default: break;
            }
            t += step;
        }
        return sampledPoints;
    }


    vec3 Curve::evaluate(float t, const EvaluationType type) const {
        switch (curveForm_) {
            case OPEN: t = glm::clamp(t, tMin_, tMax_); break;
            case LOOP: t = fmod(t - tMin_, tMax_ - tMin_) + tMin_; break;
        }
        switch (type) {
            case POSITION: return position(t);
            case VELOCITY: return velocity(t);
            case ACCELERATION: return acceleration(t);
            default: return vec3(0.0f);
        }
    }


    // TODO: ComputeCurveLength -> Evaluation resolution dependent on length?

    // TODO: GetSegmentLength


    void Curve::computeCentroid() {
        centroid_ = vec3(0.0f);
        const vector<vec3> positions = sample();
        const vector<vec3> velocities = sample();
        if (positions.size() < 2 || velocities.size() < 2) {
            return;
        }
        float totalLength = 0.0f;
        for (size_t i = 0; i < positions.size() - 1; ++i) {
            const float v1 = length(velocities[i]);
            const float v2 = length(velocities[i + 1]);
            const float segmentLength = 0.5f * (v1 + v2) * ((tMax_ - tMin_) / static_cast<float>(300));
            totalLength += segmentLength;
            centroid_ += positions[i] * segmentLength;
        }
        centroid_ = (totalLength > 0) ? centroid_ / totalLength : vec3(0.0f);
    }


    mat3 Curve::computeFrenetFrame(const float t) const {
        vec3 tangent = evaluate(t, VELOCITY);
        if (length(tangent) < 1e-6f) {
            return lastFrenetFrame_;
        }
        tangent = normalize(tangent);
        vec3 normal = evaluate(t, ACCELERATION);
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
        const bool tangentChanged = length(cross(lastFrenetFrame_[0], tangent)) > 0.01f;
        const bool normalChanged = length(cross(lastFrenetFrame_[1], normal)) > 0.01f; // NOLINT
        if (tangentChanged || normalChanged) {
            lastFrenetFrame_ = {tangent, normal, binormal};
        }
        return lastFrenetFrame_;
    }


    mat3 Curve::computeRotMinFrame(const float t) const {
        vec3 tangent = evaluate(t, VELOCITY);
        if (length(tangent) < 1e-6f) {
            return lastRotMinFrame_;
        }
        tangent = normalize(tangent);
        const vec3 v = cross(lastRotMinFrame_[0], tangent);
        const float sinTheta = length(v);
        const float cosTheta = dot(lastRotMinFrame_[0], tangent);
        vec3 normal;
        if (sinTheta > 1e-6f) {
            normal = cosTheta * lastRotMinFrame_[1] + sinTheta * cross(normalize(v), lastRotMinFrame_[1]);
            normal = normalize(normal);
        } else {
            normal = lastRotMinFrame_[1];
        }
        const vec3 binormal = normalize(cross(tangent, normal));
        lastRotMinFrame_ = mat3(tangent, normal, binormal);
        return lastRotMinFrame_;
    }


    mat3 Curve::computeCentroidFrame(const float t) const {
        vec3 tangent = evaluate(t, VELOCITY);
        if (length(tangent) < 1e-6f) {
            return lastCentroidFrame_;
        }
        tangent = normalize(tangent);
        vec3 normal = centroid_ - evaluate(t, POSITION);
        if (length(normal) < 1e-6f) {
            normal = abs(dot(tangent, vec3(0, 1, 0))) < 0.9f
                     ? vec3(0, 1, 0)
                     : vec3(1, 0, 0);
        } else {
            normal = normalize(normal);
        }
        vec3 binormal = normalize(cross(tangent, normal));
        normal = normalize(cross(binormal, tangent));
        const bool tangentChanged = length(cross(lastCentroidFrame_[0], tangent)) > 0.01f;
        const bool normalChanged = length(cross(lastCentroidFrame_[1], normal)) > 0.01f; // NOLINT
        if (tangentChanged || normalChanged) {
            lastCentroidFrame_ = {tangent, normal, binormal};
        }
        return lastCentroidFrame_;
    }

} // namespace pixelforge::geometry
