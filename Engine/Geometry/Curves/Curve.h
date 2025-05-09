#pragma once
#include <GLM/glm.hpp>
#include <vector>


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
    void SetParameterRange(float min, float max);

    /// Sets the parameter range between two curve points closest to the given target points.
    /// @param pointA The first point to project onto the curve.
    /// @param pointB The second point to project onto the curve.
    void SetParameterRange(const glm::vec3& pointA, const glm::vec3& pointB);

    /// Adds a control point to the curve.
    /// @param point The point to add.
    virtual void AddControlPoint(const glm::vec3& point);

    /// Estimates the parameter value of a curve point closest to the given target point.
    /// @param target The point to project onto the curve.
    /// @param samples Number of pre-search samples to evaluate.
    /// @param epsilon Error tolerance for refinement.
    /// @return Estimated parameter value.
    [[nodiscard]] float EstimateParameter(const glm::vec3& target, int samples = 10, float epsilon = 1e-4f) const;

    /// Gets the form of the curve (OPEN or LOOP).
    /// @return The curve form.
    [[nodiscard]] CurveForm GetForm() const;

    /// Gets the minimum parameter value.
    /// @return The tMin value.
    [[nodiscard]] float GetTMin() const;

    /// Gets the maximum parameter value.
    /// @return The tMax value.
    [[nodiscard]] float GetTMax() const;

    /// Gets the geometric centroid of the control points.
    /// @return Centroid as a vector.
    [[nodiscard]] glm::vec3 GetCentroid() const;

    /// Retrieves the control points.
    /// @return A vector of control points.
    [[nodiscard]] std::vector<glm::vec3> GetControlPoints() const;

    /// Samples the curve at evenly spaced parameter values.
    /// @param sampleCount Number of samples to generate.
    /// @param type Evaluation type: POSITION (default), VELOCITY, ACCELERATION.
    /// @return Vector of sampled curve points based on the evaluation type.
    [[nodiscard]] std::vector<glm::vec3> Sample(uint sampleCount = 300, EvaluationType type = POSITION) const;

    /// Evaluates the curve at a given parameter value.
    /// @param t The parameter value.
    /// @param type Evaluation type: POSITION (default), VELOCITY, ACCELERATION.
    /// @return Resulting curve point based on the evaluation type.
    [[nodiscard]] glm::vec3 Evaluate(float t, EvaluationType type = POSITION) const;

    /// Computes the Frenet frame at a given parameter value.
    /// @param t The parameter value.
    /// @return 3x3 matrix where columns are the tangent, normal and binormal vectors.
    [[nodiscard]] glm::mat3 ComputeFrenetFrame(float t) const;

    /// Computes a rotation-minimizing frame at a given parameter value.
    /// @param t The parameter value.
    /// @return A 3x3 rotation-minimizing frame matrix.
    [[nodiscard]] glm::mat3 ComputeRotMinFrame(float t) const;

    /// Computes a centroid-based frame at a given parameter value.
    /// @param t The parameter value.
    /// @return A 3x3 matrix aligned with the centroid vector.
    [[nodiscard]] glm::mat3 ComputeCentroidFrame(float t) const;

    /// Virtual destructor for cleanup.
    virtual ~Curve() = default;

protected:
    CurveForm curveForm; ///< Form of the curve (open or loop).
    float tMin = 0.0f;   ///< Minimum parameter value.
    float tMax = 1.0f;   ///< Maximum parameter value.
    int degree;          ///< Degree of the curve.
    glm::vec3 centroid = glm::vec3(0.0f);  ///< Geometric centroid of the curve.
    std::vector<glm::vec3> controlPoints;  ///< List of control points.
    mutable glm::mat3 lastFrenetFrame = glm::mat3(1.0f);   ///< Cached Frenet frame.
    mutable glm::mat3 lastRotMinFrame = glm::mat3(1.0f);   ///< Cached rotation-minimizing frame.
    mutable glm::mat3 lastCentroidFrame = glm::mat3(1.0f); ///< Cached centroid-based frame.

    /// Constructs a new Curve instance.
    /// @param points The control points defining the curve.
    /// @param degree The degree of the curve (if applicable).
    /// @param form The form of the curve (OPEN or LOOP).
    Curve(const std::vector<glm::vec3>& points, int degree, CurveForm form = OPEN);

    /// Computes the position of the curve at parameter t.
    /// @param t Parameter to evaluate.
    /// @return Position vector.
    [[nodiscard]] virtual glm::vec3 Position(float t) const = 0;

    /// Computes the velocity (first derivative) of the curve at parameter t.
    /// @param t Parameter to evaluate.
    /// @return Velocity vector.
    [[nodiscard]] virtual glm::vec3 Velocity(float t) const = 0;

    /// Computes the acceleration (second derivative) of the curve at parameter t.
    /// @param t Parameter to evaluate.
    /// @return Acceleration vector.
    [[nodiscard]] virtual glm::vec3 Acceleration(float t) const = 0;

    /// Computes the centroid of the curve.
    void ComputeCentroid();
};