#pragma once
#include <pixelforge/geometry/curves/curve.hpp>
#include <GLM/glm.hpp>
#include <vector>


namespace pixelforge::geometry {

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
        void addControlPoint(const glm::vec3& point) override;

        /// Adds a new control point with a specific tangent.
        /// @param point New control point.
        /// @param tangent Tangent at the control point.
        void addControlPointWithTangent(const glm::vec3& point, const glm::vec3& tangent);

        /// Replaces all control tangents with a new set.
        /// @param tangents List of tangents (must match number of control points).
        void setTangents(const std::vector<glm::vec3>& tangents);

        /// Returns the current list of control tangents.
        /// @return Vector of control tangents.
        [[nodiscard]] const std::vector<glm::vec3>& getTangents() const;

    private:
        std::vector<glm::vec3> controlTangents_; ///< Tangent vector corresponding to control points

        /// Automatically computes tangents using the Catmull-Rom method.
        void computeCatmullRomTangents();

        /// @copydoc Curve::position
        [[nodiscard]] glm::vec3 position(float t) const override;

        /// @copydoc Curve::velocity
        [[nodiscard]] glm::vec3 velocity(float t) const override;

        /// @copydoc Curve::acceleration
        [[nodiscard]] glm::vec3 acceleration(float t) const override;

        /// Evaluates the Hermite spline at a given parameter and derivative type.
        /// @param t Parameter value to evaluate.
        /// @param type Evaluation type: POSITION, VELOCITY or ACCELERATION.
        /// @return The evaluated point or derivative at t.
        [[nodiscard]] glm::vec3 evaluateSpline(float t, EvaluationType type) const;
    };

} // namespace pixelforge::geometry
