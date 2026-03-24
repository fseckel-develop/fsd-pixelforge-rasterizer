#pragma once
#include <pixelforge/geometry/curves/curve.hpp>
#include <GLM/glm.hpp>
#include <vector>


namespace pixelforge::geometry {

    /// Extension of the Curve class representing Bézier curves
    /// using the De-Casteljau algorithm for evaluations.
    class BezierCurve final : public Curve {
    public:
        /// Construct a Bézier curve from a list of control points.
        /// @param points The control points defining the curve.
        explicit BezierCurve(const std::vector<glm::vec3>& points);

        /// Adds a control point and updates the derivative data.
        ///@param point Control point to add.
        void addControlPoint(const glm::vec3& point) override;

        /// Gets the cached derivative points for the Bézier curve.
        /// @param degree 0 for first derivative, 1 for second derivative.
        /// @return Reference to the corresponding list of derivative points.
        [[nodiscard]] const std::vector<glm::vec3>& getDerivativePoints(int degree) const;

    private:
        std::vector<glm::vec3> derivativePoints_[2]; ///< Cached first and second derivative control points

        /// Computes and caches the derivative control points.
        void computeDerivativePoints();

        /// @copydoc Curve::position
        [[nodiscard]] glm::vec3 position(float t) const override;

        /// @copydoc Curve::velocity
        [[nodiscard]] glm::vec3 velocity(float t) const override;

        /// @copydoc Curve::acceleration
        [[nodiscard]] glm::vec3 acceleration(float t) const override;
    };

} // namespace pixelforge::geometry
