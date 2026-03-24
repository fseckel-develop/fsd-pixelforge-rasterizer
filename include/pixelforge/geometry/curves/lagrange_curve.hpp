#pragma once
#include <pixelforge/geometry/curves/curve.hpp>
#include <GLM/glm.hpp>
#include <vector>


namespace pixelforge::geometry {

    /// Extension of the Curve class supporting Lagrange curves.
    class LagrangeCurve final : public Curve {
    public:
        /// Constructs a Lagrange curve from a list of control points.
        /// @param points Control points to interpolate.
        explicit LagrangeCurve(const std::vector<glm::vec3>& points);

    private:
        /// Computes the Lagrange basis function (position) at index i.
        /// @param t Parameter value to evaluate.
        /// @param i Index of the basis function.
        /// @return Value of the basis function.
        [[nodiscard]] float basisFunctionForPosition(float t, int i) const;

        /// Computes the first derivative (velocity) of the Lagrange basis function.
        /// @param t Parameter value to evaluate.
        /// @param i Index of the basis function.
        /// @return First derivative of the basis function.
        [[nodiscard]] float basisFunctionForVelocity(float t, int i) const;

        /// Computes the second derivative (acceleration) of the Lagrange basis function.
        /// @param t Parameter value to evaluate.
        /// @param i Index of the basis function.
        /// @return Second derivative of the basis function.
        [[nodiscard]] float basisFunctionForAcceleration(float t, int i) const;

        /// @copydoc Curve::position
        [[nodiscard]] glm::vec3 position(float t) const override;

        /// @copydoc Curve::velocity
        [[nodiscard]] glm::vec3 velocity(float t) const override;

        /// @copydoc Curve::acceleration
        [[nodiscard]] glm::vec3 acceleration(float t) const override;

        /// Evaluates the Lagrange curve based on the evaluation type.
        /// @param t Parameter value to evaluate.
        /// @param type Evaluation type: POSITION, VELOCITY or ACCELERATION.
        /// @return The evaluated point or derivative at t.
        [[nodiscard]] glm::vec3 evaluateBasis(float t, EvaluationType type) const;
    };

} // namespace pixelforge::geometry
