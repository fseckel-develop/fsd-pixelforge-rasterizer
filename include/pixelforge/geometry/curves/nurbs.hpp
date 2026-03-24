#pragma once
#include <pixelforge/geometry/curves/bspline.hpp>
#include <GLM/glm.hpp>
#include <vector>


namespace pixelforge::geometry {

    /// Extension of the BSpline class representing a non-uniform
    /// rational B-Spline (NURBS) by adding support for weights.
    class NURBS final : public BSpline {
    public:
        /// Constructs a NURBS curve with given control points and weights and a degree.
        /// @param points The list of control points defining the curve.
        /// @param weights The weights associated with each control point.
        /// @param degree The degree of the curve.
        /// @param form Whether the curve is open or forms a loop.
        explicit NURBS(const std::vector<glm::vec3>& points, const std::vector<float>& weights, int degree, CurveForm form = OPEN);

        /// Adds a new control point with default weight of 1.0 and recomputes the knot vector.
        /// @param point New control point.
        void addControlPoint(const glm::vec3& point) override;

        /// Adds a new control point and weight and recomputes the knot vector.
        /// @param point New control point.
        /// @param weight Weight of the new control point.
        void addControlPointWithWeight(const glm::vec3& point, float weight);

        /// Gets the weights used by the curve.
        /// @return Reference to the vector of weights.
        [[nodiscard]] const std::vector<float>& getWeights() const;

    private:
        std::vector<float> weights_; ///< Weight associated with each control point.

        /// Computes a curve sample based on the evaluation type.
        /// @param t Parameter value to evaluate.
        /// @param type Evaluation type: POSITION, VELOCITY or ACCELERATION.
        /// @return The evaluated point or derivative at t.
        [[nodiscard]] glm::vec3 evaluateSpline(float t, EvaluationType type) const override;
    };

} // namespace pixelforge::geometry
