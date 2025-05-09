#pragma once
#include "Curve.h"
#include <GLM/glm.hpp>
#include <vector>


/// Extension of the Curve class representing B-Spline curves.
class BSpline : public Curve {
public:
    /// Constructs a B-spline curve with given control points and a degree.
    /// @param points The control points for the B-spline curve.
    /// @param degree The degree of the curve.
    /// @param form Whether the curve is open or forms a loop.
    explicit BSpline(const std::vector<glm::vec3>& points, int degree, CurveForm form = OPEN);

    /// Adds a control point to the B-spline curve and recomputes the knot vector.
    /// @param point The control point to add.
    void AddControlPoint(const glm::vec3& point) override;

    /// Gets the knot vector of the B-spline curve.
    /// @return Reference to a vector of floats.
    [[nodiscard]] const std::vector<float>& GetKnotVector() const;

protected:
    std::vector<float> knotVector; ///< The knot vector used for the B-spline.

    /// Finds the knot span for a given parameter t.
    /// @param t Parameter value to evaluate.
    /// @return The index of the knot span.
    [[nodiscard]] int FindKnotSpan(float t) const;

    /// Computes the B-Spline basis function (position).
    /// @param g The degree of the basis function.
    /// @param i The index of the knot span.
    /// @param t Parameter value to evaluate.
    /// @return Value of the basis function.
    [[nodiscard]] float BasisFunctionForPosition(int g, int i, float t) const;

    /// Computes the first derivative (velocity) of the B-Spline basis function.
    /// @param g The degree of the basis function.
    /// @param i The index of the knot span.
    /// @param t Parameter value to evaluate.
    /// @return First derivative of the basis function.
    [[nodiscard]] float BasisFunctionForVelocity(int g, int i, float t) const;

    /// Computes the second derivative (acceleration) of the B-Spline basis function.
    /// @param g The degree of the basis function.
    /// @param i The index of the knot span.
    /// @param t Parameter value to evaluate.
    /// @return Second derivative of the basis function.
    [[nodiscard]] float BasisFunctionForAcceleration(int g, int i, float t) const;

private:
    /// Generates the knot vector based on the control points, degree and form.
    void GenerateKnotVector();

    /// @copydoc Curve::Position
    [[nodiscard]] glm::vec3 Position(float t) const override;

    /// @copydoc Curve::Velocity
    [[nodiscard]] glm::vec3 Velocity(float t) const override;

    /// @copydoc Curve::Acceleration
    [[nodiscard]] glm::vec3 Acceleration(float t) const override;

    /// Computes a curve sample based on the evaluation type.
    /// @param t Parameter value to evaluate.
    /// @param type Evaluation type: POSITION, VELOCITY or ACCELERATION.
    /// @return The evaluated point or derivative at t.
    [[nodiscard]] virtual glm::vec3 EvaluateSpline(float t, EvaluationType type) const;
};
