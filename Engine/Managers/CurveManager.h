#pragma once
#include "InstanceManager.h"
#include "../Geometry/Curves/BezierCurve.h"
#include "../Geometry/Curves/BSpline.h"
#include "../Geometry/Curves/HermiteSpline.h"
#include "../Geometry/Curves/NURBS.h"


/// Extension of the InstanceManager class for managing
/// curve instances and ensuring their uniqueness.
class CurveManager final : public InstanceManager<Curve, CurveManager> {
public:
    /// Computes the hash for a curve instance.
    /// @param curve The curve instance to hash.
    /// @return Hash value for the curve.
    static size_t Hash(const Curve& curve) {
        size_t seed = typeid(curve).hash_code();
        CombineHashes(seed, std::hash<int>{}(curve.GetForm()));
        for (const auto& controlPoint : curve.GetControlPoints()) {
            CombineHashes(seed, std::hash<glm::vec3>{}(controlPoint));
        }
        if (auto* bezierCurve = dynamic_cast<const BezierCurve*>(&curve)) {
            CombineHashes(seed, HashCurve(*bezierCurve));
        } else if (auto* bspline = dynamic_cast<const BSpline*>(&curve)) {
            CombineHashes(seed, HashCurve(*bspline));
        } else if (auto* hermiteSpline = dynamic_cast<const HermiteSpline*>(&curve)) {
            CombineHashes(seed, HashCurve(*hermiteSpline));
        } else if (auto* nurbs = dynamic_cast<const NURBS*>(&curve)) {
            CombineHashes(seed, HashCurve(*nurbs));
        }
        return seed;
    }

    /// Compares two curve instances for equality.
    /// @param left The first curve to compare.
    /// @param right The second curve to compare.
    /// @return True if the curves are equal, false otherwise.
    static bool Equals(const Curve& left, const Curve& right) {
        if (typeid(left) != typeid(right)) return false;
        if (left.GetForm() != right.GetForm()) return false;
        if (left.GetControlPoints() != right.GetControlPoints()) return false;
        if (auto* leftBezier = dynamic_cast<const BezierCurve*>(&left)) {
            auto* rightBezier = dynamic_cast<const BezierCurve*>(&right);
            if (!rightBezier) return false;
            return leftBezier->GetDerivativePoints(0) == rightBezier->GetDerivativePoints(0) &&
                leftBezier->GetDerivativePoints(1) == rightBezier->GetDerivativePoints(1);
        }
        if (auto* leftBSpline = dynamic_cast<const BSpline*>(&left)) {
            auto* rightBSpline = dynamic_cast<const BSpline*>(&right);
            if (!rightBSpline) return false;
            return leftBSpline->GetKnotVector() == rightBSpline->GetKnotVector();
        }
        if (auto* leftHermite = dynamic_cast<const HermiteSpline*>(&left)) {
            auto* rightHermite = dynamic_cast<const HermiteSpline*>(&right);
            if (!rightHermite) return false;
            return leftHermite->GetTangents() == rightHermite->GetTangents();
        }
        if (auto* leftNurbs = dynamic_cast<const NURBS*>(&left)) {
            auto* rightNurbs = dynamic_cast<const NURBS*>(&right);
            if (!rightNurbs) return false;
            return leftNurbs->GetKnotVector() == rightNurbs->GetKnotVector() &&
                leftNurbs->GetWeights() == rightNurbs->GetWeights();
        }
        return true;
    }

private:
    /// Computes the hash for a Bézier curve instance.
    /// @param curve Bézier curve instance to hash.
    /// @return The computed hash value for the Bézier curve instance.
    static size_t HashCurve(const BezierCurve& curve) {
        size_t seed = 0;
        for (const auto& derivativePoint : curve.GetDerivativePoints(0)) {
            CombineHashes(seed, std::hash<glm::vec3>{}(derivativePoint));
        }
        for (const auto& derivativePoint : curve.GetDerivativePoints(1)) {
            CombineHashes(seed, std::hash<glm::vec3>{}(derivativePoint));
        }
        return seed;
    }

    /// Computes the hash for a B-spline curve instance.
    /// @param curve B-spline curve instance to hash.
    /// @return The computed hash value for the B-spline curve instance.
    static size_t HashCurve(const BSpline& curve) {
        size_t seed = 0;
        for (const auto& knot : curve.GetKnotVector()) {
            CombineHashes(seed, std::hash<float>{}(knot));
        }
        return seed;
    }

    /// Computes the hash for a Hermite spline curve instance.
    /// @param curve Hermite spline curve instance to hash.
    /// @return The computed hash value for the Hermite spline curve instance.
    static size_t HashCurve(const HermiteSpline& curve) {
        size_t seed = 0;
        for (const auto& tangent : curve.GetTangents()) {
            CombineHashes(seed, std::hash<glm::vec3>{}(tangent));
        }
        return seed;
    }

    /// Computes the hash for a NURBS curve instance.
    /// @param curve NURBS curve instance to hash.
    /// @return The computed hash value for the NURBS curve instance.
    static size_t HashCurve(const NURBS& curve) {
        size_t seed = 0;
        for (const auto& knot : curve.GetKnotVector()) {
            CombineHashes(seed, std::hash<float>{}(knot));
        }
        for (const auto& weights : curve.GetWeights()) {
            CombineHashes(seed, std::hash<float>{}(weights));
        }
        return seed;
    }
};
