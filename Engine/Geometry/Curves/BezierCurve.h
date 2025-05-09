#pragma once
#include "Curve.h"
#include <GLM/glm.hpp>
#include <vector>


/// Extension of the Curve class representing Bézier curves
/// using the De-Casteljau algorithm for evaluations.
class BezierCurve final : public Curve {
public:
    /// Construct a Bézier curve from a list of control points.
    /// @param points The control points defining the curve.
    explicit BezierCurve(const std::vector<glm::vec3>& points);

    /// Adds a control point and updates the derivative data.
    ///@param point Control point to add.
    void AddControlPoint(const glm::vec3& point) override;

    /// Gets the cached derivative points for the Bézier curve.
    /// @param degree 0 for first derivative, 1 for second derivative.
    /// @return Reference to the corresponding list of derivative points.
    [[nodiscard]] const std::vector<glm::vec3>& GetDerivativePoints(int degree) const;

private:
    std::vector<glm::vec3> derivativePoints[2]; ///< Cached first and second derivative control points

    /// Computes and caches the derivative control points.
    void ComputeDerivativePoints();

    /// @copydoc Curve::Position
    [[nodiscard]] glm::vec3 Position(float t) const override;

    /// @copydoc Curve::Velocity
    [[nodiscard]] glm::vec3 Velocity(float t) const override;

    /// @copydoc Curve::Acceleration
    [[nodiscard]] glm::vec3 Acceleration(float t) const override;
};
