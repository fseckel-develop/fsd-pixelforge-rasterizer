#pragma once
#include "Curve.h"
#include <GLM/glm.hpp>
#include <vector>


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
    [[nodiscard]] float BasisFunctionForPosition(float t, int i) const;

    /// Computes the first derivative (velocity) of the Lagrange basis function.
    /// @param t Parameter value to evaluate.
    /// @param i Index of the basis function.
    /// @return First derivative of the basis function.
    [[nodiscard]] float BasisFunctionForVelocity(float t, int i) const;

    /// Computes the second derivative (acceleration) of the Lagrange basis function.
    /// @param t Parameter value to evaluate.
    /// @param i Index of the basis function.
    /// @return Second derivative of the basis function.
    [[nodiscard]] float BasisFunctionForAcceleration(float t, int i) const;

    /// @copydoc Curve::Position
    [[nodiscard]] glm::vec3 Position(float t) const override;

    /// @copydoc Curve::Velocity
    [[nodiscard]] glm::vec3 Velocity(float t) const override;

    /// @copydoc Curve::Acceleration
    [[nodiscard]] glm::vec3 Acceleration(float t) const override;

    /// Evaluates the Lagrange curve based on the evaluation type.
    /// @param t Parameter value to evaluate.
    /// @param type Evaluation type: POSITION, VELOCITY or ACCELERATION.
    /// @return The evaluated point or derivative at t.
    [[nodiscard]] glm::vec3 EvaluateBasis(float t, EvaluationType type) const;
};
