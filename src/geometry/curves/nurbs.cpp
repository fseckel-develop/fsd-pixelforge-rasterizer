#include <pixelforge/geometry/curves/nurbs.hpp>
#include <iostream>


namespace pixelforge::geometry {

    using std::vector;
    using glm::vec3;


    NURBS::NURBS(const vector<vec3>& points, const vector<float>& weights, const int degree, const CurveForm form):
        BSpline(points, degree, form),
        weights_(weights) {
        if (curveForm_ == LOOP) {
            for (int i = 0; i < this->degree_; i++) {
                this->weights_.push_back(weights[i]);
            }
        }
        if (controlPoints_.size() != weights.size()) {
            std::cerr << "Number of weights must match number of control points for NURBS!" << std::endl;
            this->weights_ = vector(controlPoints_.size(), 1.0f);
        }
    }


    void NURBS::addControlPoint(const vec3& point) {
        addControlPointWithWeight(point, 1.0f);
    }


    void NURBS::addControlPointWithWeight(const vec3& point, const float weight) {
        BSpline::addControlPoint(point);
        weights_.push_back(weight);
    }



    const vector<float>& NURBS::getWeights() const {
        return weights_;
    }


    vec3 NURBS::evaluateSpline(const float t, const EvaluationType type) const {
        if (controlPoints_.empty() || knotVector_.empty()) {
            std::cerr << "NURBS is not initialized correctly!" << std::endl;
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
        vector N(degree_ + 1, 0.0f), dN(degree_ + 1, 0.0f), ddN(degree_ + 1, 0.0f);
        for (int i = 0; i <= degree_; i++) {
            N[i] = basisFunctionForPosition(degree_, span - degree_ + i, tMod);
            if (type != POSITION) {
                dN[i] = basisFunctionForVelocity(degree_, span - degree_ + i, tMod);
            }
            if (type == ACCELERATION) {
                ddN[i] = basisFunctionForAcceleration(degree_, span - degree_ + i, tMod);
            }
        }
        vec3 numP(0.0f), numV(0.0f), numA(0.0f);
        float denomP = 0.0f, denomV = 0.0f, denomA = 0.0f;
        for (int i = 0; i <= degree_; i++) {
            int index = (span - degree_ + i) % static_cast<int>(controlPoints_.size());
            float weight = weights_[index];
            numP += N[i] * weight * controlPoints_[index];
            denomP += N[i] * weight;
            if (type != POSITION) {
                numV += dN[i] * weight * controlPoints_[index];
                denomV += dN[i] * weight;
            }
            if (type == ACCELERATION) {
                numA += ddN[i] * weight * controlPoints_[index];
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

} // namespace pixelforge::geometry
