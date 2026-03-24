#pragma once
#include <pixelforge/geometry/curves/curve.hpp>
#include <GLM/glm.hpp>
#include <vector>


namespace pixelforge::geometry {

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
        void addControlPoint(const glm::vec3& point) override;

        /// Gets the knot vector of the B-spline curve.
        /// @return Reference to a vector of floats.
        [[nodiscard]] const std::vector<float>& getKnotVector() const;

    protected:
        std::vector<float> knotVector_; ///< The knot vector used for the B-spline.

        /// Finds the knot span for a given parameter t.
        /// @param t Parameter value to evaluate.
        /// @return The index of the knot span.
        [[nodiscard]] int findKnotSpan(float t) const;

        /// Computes the B-Spline basis function (position).
        /// @param g The degree of the basis function.
        /// @param i The index of the knot span.
        /// @param t Parameter value to evaluate.
        /// @return Value of the basis function.
        [[nodiscard]] float basisFunctionForPosition(int g, int i, float t) const;

        /// Computes the first derivative (velocity) of the B-Spline basis function.
        /// @param g The degree of the basis function.
        /// @param i The index of the knot span.
        /// @param t Parameter value to evaluate.
        /// @return First derivative of the basis function.
        [[nodiscard]] float basisFunctionForVelocity(int g, int i, float t) const;

        /// Computes the second derivative (acceleration) of the B-Spline basis function.
        /// @param g The degree of the basis function.
        /// @param i The index of the knot span.
        /// @param t Parameter value to evaluate.
        /// @return Second derivative of the basis function.
        [[nodiscard]] float basisFunctionForAcceleration(int g, int i, float t) const;

    private:
        /// Generates the knot vector based on the control points, degree and form.
        void generateKnotVector();

        /// @copydoc Curve::position
        [[nodiscard]] glm::vec3 position(float t) const override;

        /// @copydoc Curve::velocity
        [[nodiscard]] glm::vec3 velocity(float t) const override;

        /// @copydoc Curve::acceleration
        [[nodiscard]] glm::vec3 acceleration(float t) const override;

        /// Computes a curve sample based on the evaluation type.
        /// @param t Parameter value to evaluate.
        /// @param type Evaluation type: POSITION, VELOCITY or ACCELERATION.
        /// @return The evaluated point or derivative at t.
        [[nodiscard]] virtual glm::vec3 evaluateSpline(float t, EvaluationType type) const;
    };

} // namespace pixelforge::geometry
