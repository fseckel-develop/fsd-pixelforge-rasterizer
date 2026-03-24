#include <pixelforge/geometry/curves/bezier_curve.hpp>
#include <iostream>


namespace pixelforge::geometry {

    using std::vector;
    using std::cerr;
    using std::endl;
    using glm::vec3;


    BezierCurve::BezierCurve(const vector<vec3>& points):
    Curve(points, static_cast<int>(points.size()) - 1, OPEN) {
        computeDerivativePoints();
    }


    void BezierCurve::addControlPoint(const vec3& point) {
        Curve::addControlPoint(point);
        computeDerivativePoints();
    }


    const vector<vec3>& BezierCurve::getDerivativePoints(const int degree) const {
        return derivativePoints_[glm::clamp(degree, 0, 1)];
    }


    void BezierCurve::computeDerivativePoints() {
        int n = degree_;
        if (n < 1) return;
        derivativePoints_[0].clear();
        derivativePoints_[0].resize(n);
        for (int i = 0; i < n; i++) {
            derivativePoints_[0][i] = static_cast<float>(n) * (controlPoints_[i + 1] - controlPoints_[i]);
        }
        if (--n < 1) return;
        derivativePoints_[1].clear();
        derivativePoints_[1].resize(n);
        for (int i = 0; i < n; i++) {
            derivativePoints_[1][i] = static_cast<float>(n) * (derivativePoints_[0][i + 1] - derivativePoints_[0][i]);
        }
    }



    vec3 BezierCurve::position(const float t) const {
        if (controlPoints_.size() < 2) {
            cerr << "Not enough data to evaluate Bezier curve." << endl;
            return vec3(0.0f);
        }
        std::vector<vec3> temp = controlPoints_;
        for (int r = 1; r <= degree_; r++) {
            for (int i = 0; i <= degree_ - r; i++) {
                temp[i] = (1.0f - t) * temp[i] + t * temp[i + 1];
            }
        }
        return temp[0];
    }


    vec3 BezierCurve::velocity(const float t) const {
        if (derivativePoints_[0].size() < 2) {
            cerr << "Not enough data for second Bezier curve derivative." << endl;
            return vec3(0.0f);
        }
        std::vector<vec3> temp = derivativePoints_[0];
        const int n = degree_ - 1;
        for (int r = 1; r <= n; r++) {
            for (int i = 0; i <= n - r; i++) {
                temp[i] = (1.0f - t) * temp[i] + t * temp[i + 1];
            }
        }
        return temp[0];
    }


    vec3 BezierCurve::acceleration(const float t) const {
        if (derivativePoints_[1].size() < 2) {
            cerr << "Not enough data for first Bezier curve derivative." << endl;
            return vec3(0.0f);
        }
        vector<vec3> temp = derivativePoints_[1];
        const int n = degree_ - 2;
        for (int r = 1; r <= n; r++) {
            for (int i = 0; i <= n - r; i++) {
                temp[i] = (1.0f - t) * temp[i] + t * temp[i + 1];
            }
        }
        return temp[0];
    }

} // namespace pixelforge::geometry
