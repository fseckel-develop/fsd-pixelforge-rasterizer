#include <pixelforge/geometry/curves/hermite_spline.hpp>
#include <iostream>


namespace pixelforge::geometry {

    using std::cerr;
    using std::endl;
    using glm::vec3;


    HermiteSpline::HermiteSpline(const std::vector<vec3>& points, const std::vector<vec3>& tangents, const CurveForm form):
        Curve(points, static_cast<int>(points.size()) - 1, form),
        controlTangents_(tangents) {
        if (controlPoints_.size() != controlTangents_.size()) {
            cerr << "Number of tangents must match number of control points for Hermite Spline!" << endl;
            computeCatmullRomTangents();
        }
        computeCentroid();
    }


    HermiteSpline::HermiteSpline(const std::vector<vec3>& points, const CurveForm form):
        Curve(points, static_cast<int>(points.size()) - 1, form),
        controlTangents_({}) {
        computeCatmullRomTangents();
    }


    void HermiteSpline::addControlPoint(const vec3& point) {
        Curve::addControlPoint(point);
        computeCatmullRomTangents();
    }


    void HermiteSpline::addControlPointWithTangent(const vec3& point, const vec3& tangent) {
        Curve::addControlPoint(point);
        controlTangents_.push_back(tangent);
    }


    void HermiteSpline::setTangents(const std::vector<vec3>& tangents) {
        this->controlTangents_ = tangents;
    }


    const std::vector<vec3>& HermiteSpline::getTangents() const {
        return controlTangents_;
    }


    void HermiteSpline::computeCatmullRomTangents() {
        const int n = static_cast<int>(controlPoints_.size());
        if (n < 2) return;
        controlTangents_.resize(n);
        for (int i = 0; i < n; i++) {
            const int prev = (i - 1 + n) % n;
            const int next = (i + 1) % n;
            controlTangents_[i] = 0.5f * (controlPoints_[next] - controlPoints_[prev]);
        }
    }


    vec3 HermiteSpline::position(const float t) const {
        return evaluateSpline(t, POSITION);
    }


    vec3 HermiteSpline::velocity(const float t) const {
        return evaluateSpline(t, VELOCITY);
    }


    vec3 HermiteSpline::acceleration(const float t) const {
        return evaluateSpline(t, ACCELERATION);
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


    vec3 HermiteSpline::evaluateSpline(const float t, const EvaluationType type) const {
        if (controlPoints_.size() < 2 || controlTangents_.size() < 2) {
            cerr << "Not enough data to evaluate Hermite spline." << endl;
            return vec3(0.0f);
        }
        const int pointCount = static_cast<int>(controlPoints_.size());
        int i0 = 0;
        int i1 = 1;
        float localT = 0.0f;
        if (curveForm_ == LOOP) {
            const float segmentF = t * static_cast<float>(pointCount);
            const int segment = static_cast<int>(segmentF) % pointCount;
            localT = segmentF - static_cast<float>(static_cast<int>(segmentF));
            i0 = segment;
            i1 = (segment + 1) % pointCount;
        } else {
            const int segmentCount = pointCount - 1;

            if (t >= 1.0f) {
                i0 = segmentCount - 1;
                i1 = segmentCount;
                localT = 1.0f;
            } else {
                const float segmentF = t * static_cast<float>(segmentCount);
                const int segment = static_cast<int>(segmentF);
                localT = segmentF - static_cast<float>(segment);
                i0 = segment;
                i1 = segment + 1;
            }
        }
        const vec3 p0 = controlPoints_[i0];
        const vec3 p1 = controlPoints_[i1];
        const vec3 m0 = controlTangents_[i0];
        const vec3 m1 = controlTangents_[i1];
        switch (type) {
            case POSITION: return h00(localT) * p0 + h10(localT) * m0 + h01(localT) * p1 + h11(localT) * m1;
            case VELOCITY: return dh00(localT) * p0 + dh10(localT) * m0 + dh01(localT) * p1 + dh11(localT) * m1;
            case ACCELERATION: return d2h00(localT) * p0 + d2h10(localT) * m0 + d2h01(localT) * p1 + d2h11(localT) * m1;
            default: return vec3(0.0f); // NOLINT
        }
    }

} // namespace pixelforge::geometry
