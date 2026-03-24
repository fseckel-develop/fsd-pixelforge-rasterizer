#include <pixelforge/geometry/curves/bspline.hpp>
#include <iostream>


namespace pixelforge::geometry {

    using std::vector;
    using std::cerr;
    using std::endl;
    using glm::vec3;


    BSpline::BSpline(const vector<vec3>& points, const int degree, const CurveForm form):
        Curve(points, degree, form) {
        computeCentroid();
        if (!controlPoints_.empty() && controlPoints_.size() <= static_cast<size_t>(this->degree_)) {
            cerr << "Invalid degree for BSpline" << endl;
            this->degree_ = static_cast<int>(controlPoints_.size()) - 1;
        }
        if (curveForm_ == LOOP) {
            for (int i = 0; i < degree; i++) {
                controlPoints_.push_back(controlPoints_[i]);
            }
        }
        generateKnotVector();
    }


    void BSpline::addControlPoint(const glm::vec3 &point) {
        controlPoints_.push_back(point);
        generateKnotVector();
    }


    const vector<float>& BSpline::getKnotVector() const {
        return knotVector_;
    }


    int BSpline::findKnotSpan(float t) const {
        const int n = static_cast<int>(controlPoints_.size()) - 1;
        if (curveForm_ == LOOP) {
            const float period = knotVector_.back() - knotVector_.front();
            t = fmod(t - knotVector_.front(), period) + knotVector_.front();
        }
        if (knotVector_[n + 1] <= t) {
            return n;
        }
        if (t <= knotVector_[degree_]) {
            return degree_;
        }
        for (int i = degree_; i <= n; i++) {
            if (knotVector_[i] <= t && t < knotVector_[i + 1]) {
                return i;
            }
        }
        return n;
    }


    float BSpline::basisFunctionForPosition(const int g, const int i, const float t) const {
        std::vector N(g + 1, 0.0f);
        for (int j = 0; j <= g; ++j) {
            N[j] = (knotVector_[i + j] <= t && t < knotVector_[i + j + 1])
                ? 1.0f
                : 0.0f;
        }
        for (int d = 1; d <= g; ++d) {
            for (int j = 0; j <= g - d; ++j) {
                const float denomA = knotVector_[i + j + d] - knotVector_[i + j];
                const float denomB = knotVector_[i + j + d + 1] - knotVector_[i + j + 1];
                const float termA = (denomA != 0.0f)
                    ? ((t - knotVector_[i + j]) / denomA) * N[j]
                    : 0.0f;
                const float termB = (denomB != 0.0f)
                    ? ((knotVector_[i + j + d + 1] - t) / denomB) * N[j + 1]
                    : 0.0f;
                N[j] = termA + termB;
            }
        }
        return N[0];
    }


    float BSpline::basisFunctionForVelocity(const int g, const int i, const float t) const {
        const float denomA = knotVector_[i + g] - knotVector_[i];
        const float denomB = knotVector_[i + g + 1] - knotVector_[i + 1];
        const float termA = (denomA != 0.0f)
            ? static_cast<float>(g) / denomA * basisFunctionForPosition(g - 1, i, t)
            : 0.0f;
        const float termB = (denomB != 0.0f)
            ? static_cast<float>(g) / denomB * basisFunctionForPosition(g - 1, i + 1, t)
            : 0.0f;
        return termA - termB;
    }


    float BSpline::basisFunctionForAcceleration(const int g, const int i, const float t) const {
        const float denomA = knotVector_[i + g] - knotVector_[i];
        const float denomB = knotVector_[i + g + 1] - knotVector_[i + 1];
        const float denomC = knotVector_[i + g + 2] - knotVector_[i + 2];
        const float termA = (denomA != 0.0f && denomB != 0.0f)
            ? static_cast<float>(g * (g - 1)) / (denomA * denomB) * basisFunctionForPosition(g - 2, i, t)
            : 0.0f;
        const float termB = (denomB != 0.0f && denomC != 0.0f)
            ? static_cast<float>(g * (g - 1)) / (denomB * denomC) * basisFunctionForPosition(g - 2, i + 1, t)
            : 0.0f;
        return termA - termB;
    }


    void BSpline::generateKnotVector() {
        const int K = static_cast<int>(controlPoints_.size());
        knotVector_.clear();
        if (curveForm_ == LOOP) {
            const int numKnots = K + degree_ + 1;
            for (int i = 0; i < numKnots; i++) {
                knotVector_.push_back(static_cast<float>(i - degree_));
            }
        } else {
            const int n = K - degree_;
            for (int i = 0; i <= n; i++) {
                if (i == 0 || i == n) {
                    for (int j = 0; j <= degree_; j++) {
                        knotVector_.push_back(static_cast<float>(i));
                    }
                } else {
                    knotVector_.push_back(static_cast<float>(i));
                }
            }
        }
        const auto normalizer = static_cast<float>(K - degree_);
        for (float& knot : knotVector_) {
            knot = knot / normalizer;
        }
    }


    vec3 BSpline::position(const float t) const {
        return evaluateSpline(t, POSITION);
    }


    vec3 BSpline::velocity(const float t) const {
        return evaluateSpline(t, VELOCITY);
    }


    vec3 BSpline::acceleration(const float t) const {
        return evaluateSpline(t, ACCELERATION);
    }


    vec3 BSpline::evaluateSpline(const float t, const EvaluationType type) const {
        if (controlPoints_.empty() || knotVector_.empty()) {
            std::cerr << "BSpline is not initialized correctly!" << std::endl;
            return vec3(0.0f);
        }
        float tMod = t;
        if (curveForm_ == LOOP) {
            const float period = knotVector_.back() - knotVector_.front();
            tMod = fmod(tMod - knotVector_.front(), period) + knotVector_.front();
        }
        else if (t == tMax_) {
            return controlPoints_.back();
        }
        const int span = findKnotSpan(tMod);
        std::vector N(degree_ + 1, 0.0f);
        for (int i = 0; i <= degree_; i++) {
            switch (type) {
                case POSITION: N[i] = basisFunctionForPosition(degree_, span - degree_ + i, tMod); break;
                case VELOCITY: N[i] = basisFunctionForVelocity(degree_, span - degree_ + i, tMod); break;
                case ACCELERATION: N[i] = basisFunctionForAcceleration(degree_, span - degree_ + i, tMod); break;
            }
        }
        vec3 result(0.0f);
        for (int i = 0; i <= degree_; i++) {
            const int index = (span - degree_ + i) % static_cast<int>(controlPoints_.size());
            result += N[i] * controlPoints_[index];
        }
        return result;
    }

} // namespace pixelforge::geometry
