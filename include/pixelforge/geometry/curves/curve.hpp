#pragma once
#include <GLM/glm.hpp>
#include <vector>


namespace pixelforge::geometry {

    /// Abstract base class which represents a parametric curve, supporting different
    /// evaluation types (position, velocity, acceleration) for sampling, different
    /// ways to set the parameter range and methods for computing of geometric frames.
    class Curve {
    public:
        /// Enumeration of possible curve forms.
        enum CurveForm { LOOP, OPEN };

        /// Enumeration of evaluation types for sampling or analysis.
        enum EvaluationType { POSITION, VELOCITY, ACCELERATION };

        /// Sets the parameter range of the parametric curve.
        /// @param min Minimum parameter value.
        /// @param max Maximum parameter value.
        void setParameterRange(float min, float max);

        /// Sets the parameter range between two curve points closest to the given target points.
        /// @param pointA The first point to project onto the curve.
        /// @param pointB The second point to project onto the curve.
        void setParameterRange(const glm::vec3& pointA, const glm::vec3& pointB);

        /// Adds a control point to the curve.
        /// @param point The point to add.
        virtual void addControlPoint(const glm::vec3& point);

        /// Estimates the parameter value of a curve point closest to the given target point.
        /// @param target The point to project onto the curve.
        /// @param samples Number of pre-search samples to evaluate.
        /// @param epsilon Error tolerance for refinement.
        /// @return Estimated parameter value.
        [[nodiscard]] float estimateParameter(const glm::vec3& target, int samples = 10, float epsilon = 1e-4f) const;

        /// Gets the form of the curve (OPEN or LOOP).
        /// @return The curve form.
        [[nodiscard]] CurveForm getForm() const;

        /// Gets the minimum parameter value.
        /// @return The tMin value.
        [[nodiscard]] float getTMin() const;

        /// Gets the maximum parameter value.
        /// @return The tMax value.
        [[nodiscard]] float getTMax() const;

        /// Gets the geometric centroid of the control points.
        /// @return Centroid as a vector.
        [[nodiscard]] glm::vec3 getCentroid() const;

        /// Retrieves the control points.
        /// @return A vector of control points.
        [[nodiscard]] std::vector<glm::vec3> getControlPoints() const;

        /// Samples the curve at evenly spaced parameter values.
        /// @param sampleCount Number of samples to generate.
        /// @param type Evaluation type: POSITION (default), VELOCITY, ACCELERATION.
        /// @return Vector of sampled curve points based on the evaluation type.
        [[nodiscard]] std::vector<glm::vec3> sample(uint sampleCount = 300, EvaluationType type = POSITION) const;

        /// Evaluates the curve at a given parameter value.
        /// @param t The parameter value.
        /// @param type Evaluation type: POSITION (default), VELOCITY, ACCELERATION.
        /// @return Resulting curve point based on the evaluation type.
        [[nodiscard]] glm::vec3 evaluate(float t, EvaluationType type = POSITION) const;

        /// Computes the Frenet frame at a given parameter value.
        /// @param t The parameter value.
        /// @return 3x3 matrix where columns are the tangent, normal and binormal vectors.
        [[nodiscard]] glm::mat3 computeFrenetFrame(float t) const;

        /// Computes a rotation-minimizing frame at a given parameter value.
        /// @param t The parameter value.
        /// @return A 3x3 rotation-minimizing frame matrix.
        [[nodiscard]] glm::mat3 computeRotMinFrame(float t) const;

        /// Computes a centroid-based frame at a given parameter value.
        /// @param t The parameter value.
        /// @return A 3x3 matrix aligned with the centroid vector.
        [[nodiscard]] glm::mat3 computeCentroidFrame(float t) const;

        /// Virtual destructor for cleanup.
        virtual ~Curve() = default;

    protected:
        CurveForm curveForm_; ///< Form of the curve (open or loop).
        float tMin_ = 0.0f;   ///< Minimum parameter value.
        float tMax_ = 1.0f;   ///< Maximum parameter value.
        int degree_;          ///< Degree of the curve.
        glm::vec3 centroid_ = glm::vec3(0.0f);  ///< Geometric centroid of the curve.
        std::vector<glm::vec3> controlPoints_;  ///< List of control points.
        mutable glm::mat3 lastFrenetFrame_ = glm::mat3(1.0f);   ///< Cached Frenet frame.
        mutable glm::mat3 lastRotMinFrame_ = glm::mat3(1.0f);   ///< Cached rotation-minimizing frame.
        mutable glm::mat3 lastCentroidFrame_ = glm::mat3(1.0f); ///< Cached centroid-based frame.

        /// Constructs a new Curve instance.
        /// @param points The control points defining the curve.
        /// @param degree The degree of the curve (if applicable).
        /// @param form The form of the curve (OPEN or LOOP).
        Curve(const std::vector<glm::vec3>& points, int degree, CurveForm form = OPEN);

        /// Computes the position of the curve at parameter t.
        /// @param t Parameter to evaluate.
        /// @return Position vector.
        [[nodiscard]] virtual glm::vec3 position(float t) const = 0;

        /// Computes the velocity (first derivative) of the curve at parameter t.
        /// @param t Parameter to evaluate.
        /// @return Velocity vector.
        [[nodiscard]] virtual glm::vec3 velocity(float t) const = 0;

        /// Computes the acceleration (second derivative) of the curve at parameter t.
        /// @param t Parameter to evaluate.
        /// @return Acceleration vector.
        [[nodiscard]] virtual glm::vec3 acceleration(float t) const = 0;

        /// Computes the centroid of the curve.
        void computeCentroid();
    };

} // namespace pixelforge::geometry
