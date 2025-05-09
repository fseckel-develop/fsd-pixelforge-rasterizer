#pragma once
#include "Curve.h"
#include <GLM/glm.hpp>
#include <vector>


/// Extension of the Curve class supporting Hermite splines by
/// interpolation with tangents and Catmull-Rom approximation.
class HermiteSpline final : public Curve {
public:
    /// Constructs a Hermite spline with given control points and tangents.
    /// @param points Control points to interpolate.
    /// @param tangents Tangents at each control point.
    /// @param form Whether the curve is open or forms a loop.
    explicit HermiteSpline(const std::vector<glm::vec3>& points, std::vector<glm::vec3>& tangents, CurveForm form = OPEN);

    /// Constructs a Hermite spline with automatic Catmull-Rom tangents.
    /// @param points Control points to interpolate.
    /// @param form Whether the curve is open or forms a loop.
    explicit HermiteSpline(const std::vector<glm::vec3>& points, CurveForm form = OPEN);

    /// Adds a new control point and recomputes tangents.
    /// @param point New control point.
    void AddControlPoint(const glm::vec3& point) override;

    /// Adds a new control point with a specific tangent.
    /// @param point New control point.
    /// @param tangent Tangent at the control point.
    void AddControlPointWithTangent(const glm::vec3& point, const glm::vec3& tangent);

    /// Replaces all control tangents with a new set.
    /// @param tangents List of tangents (must match number of control points).
    void SetTangents(const std::vector<glm::vec3>& tangents);

    /// Returns the current list of control tangents.
    /// @return Vector of control tangents.
    [[nodiscard]] const std::vector<glm::vec3>& GetTangents() const;

private:
    std::vector<glm::vec3> controlTangents; ///< Tangent vector corresponding to control points

    /// Automatically computes tangents using the Catmull-Rom method.
    void ComputeCatmullRomTangents();

    /// @copydoc Curve::Position
    [[nodiscard]] glm::vec3 Position(float t) const override;

    /// @copydoc Curve::Velocity
    [[nodiscard]] glm::vec3 Velocity(float t) const override;

    /// @copydoc Curve::Acceleration
    [[nodiscard]] glm::vec3 Acceleration(float t) const override;

    /// Evaluates the Hermite spline at a given parameter and derivative type.
    /// @param t Parameter value to evaluate.
    /// @param type Evaluation type: POSITION, VELOCITY or ACCELERATION.
    /// @return The evaluated point or derivative at t.
    [[nodiscard]] glm::vec3 EvaluateSpline(float t, EvaluationType type) const;
};
