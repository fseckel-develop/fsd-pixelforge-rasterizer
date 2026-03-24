#pragma once
#include <pixelforge/managers/instance_manager.hpp>
#include <pixelforge/geometry/curves/bezier_curve.hpp>
#include <pixelforge/geometry/curves/bspline.hpp>
#include <pixelforge/geometry/curves/hermite_spline.hpp>
#include <pixelforge/geometry/curves/nurbs.hpp>


namespace pixelforge::management {

    /// Extension of the InstanceManager class for managing
    /// curve instances and ensuring their uniqueness.
    class CurveManager final : public InstanceManager<geometry::Curve, CurveManager> {
    public:
        /// Computes the hash for a curve instance.
        /// @param curve The curve instance to hash.
        /// @return Hash value for the curve.
        static size_t hash(const geometry::Curve& curve) {
            size_t seed = typeid(curve).hash_code();
            combineHashes(seed, std::hash<int>{}(curve.getForm()));
            for (const auto& controlPoint : curve.getControlPoints()) {
                combineHashes(seed, std::hash<glm::vec3>{}(controlPoint));
            }
            if (auto* bezierCurve = dynamic_cast<const geometry::BezierCurve*>(&curve)) {
                combineHashes(seed, hashCurve(*bezierCurve));
            } else if (auto* bspline = dynamic_cast<const geometry::BSpline*>(&curve)) {
                combineHashes(seed, hashCurve(*bspline));
            } else if (auto* hermiteSpline = dynamic_cast<const geometry::HermiteSpline*>(&curve)) {
                combineHashes(seed, hashCurve(*hermiteSpline));
            } else if (auto* nurbs = dynamic_cast<const geometry::NURBS*>(&curve)) {
                combineHashes(seed, hashCurve(*nurbs));
            }
            return seed;
        }

        /// Compares two curve instances for equality.
        /// @param left The first curve to compare.
        /// @param right The second curve to compare.
        /// @return True if the curves are equal, false otherwise.
        static bool equals(const geometry::Curve& left, const geometry::Curve& right) {
            if (typeid(left) != typeid(right)) return false;
            if (left.getForm() != right.getForm()) return false;
            if (left.getControlPoints() != right.getControlPoints()) return false;
            if (auto* leftBezier = dynamic_cast<const geometry::BezierCurve*>(&left)) {
                auto* rightBezier = dynamic_cast<const geometry::BezierCurve*>(&right);
                if (!rightBezier) return false;
                return leftBezier->getDerivativePoints(0) == rightBezier->getDerivativePoints(0) &&
                    leftBezier->getDerivativePoints(1) == rightBezier->getDerivativePoints(1);
            }
            if (auto* leftBSpline = dynamic_cast<const geometry::BSpline*>(&left)) {
                auto* rightBSpline = dynamic_cast<const geometry::BSpline*>(&right);
                if (!rightBSpline) return false;
                return leftBSpline->getKnotVector() == rightBSpline->getKnotVector();
            }
            if (auto* leftHermite = dynamic_cast<const geometry::HermiteSpline*>(&left)) {
                auto* rightHermite = dynamic_cast<const geometry::HermiteSpline*>(&right);
                if (!rightHermite) return false;
                return leftHermite->getTangents() == rightHermite->getTangents();
            }
            if (auto* leftNurbs = dynamic_cast<const geometry::NURBS*>(&left)) {
                auto* rightNurbs = dynamic_cast<const geometry::NURBS*>(&right);
                if (!rightNurbs) return false;
                return leftNurbs->getKnotVector() == rightNurbs->getKnotVector() &&
                    leftNurbs->getWeights() == rightNurbs->getWeights();
            }
            return true;
        }

    private:
        /// Computes the hash for a Bézier curve instance.
        /// @param curve Bézier curve instance to hash.
        /// @return The computed hash value for the Bézier curve instance.
        static size_t hashCurve(const geometry::BezierCurve& curve) {
            size_t seed = 0;
            for (const auto& derivativePoint : curve.getDerivativePoints(0)) {
                combineHashes(seed, std::hash<glm::vec3>{}(derivativePoint));
            }
            for (const auto& derivativePoint : curve.getDerivativePoints(1)) {
                combineHashes(seed, std::hash<glm::vec3>{}(derivativePoint));
            }
            return seed;
        }

        /// Computes the hash for a B-spline curve instance.
        /// @param curve B-spline curve instance to hash.
        /// @return The computed hash value for the B-spline curve instance.
        static size_t hashCurve(const geometry::BSpline& curve) {
            size_t seed = 0;
            for (const auto& knot : curve.getKnotVector()) {
                combineHashes(seed, std::hash<float>{}(knot));
            }
            return seed;
        }

        /// Computes the hash for a Hermite spline curve instance.
        /// @param curve Hermite spline curve instance to hash.
        /// @return The computed hash value for the Hermite spline curve instance.
        static size_t hashCurve(const geometry::HermiteSpline& curve) {
            size_t seed = 0;
            for (const auto& tangent : curve.getTangents()) {
                combineHashes(seed, std::hash<glm::vec3>{}(tangent));
            }
            return seed;
        }

        /// Computes the hash for a NURBS curve instance.
        /// @param curve NURBS curve instance to hash.
        /// @return The computed hash value for the NURBS curve instance.
        static size_t hashCurve(const geometry::NURBS& curve) {
            size_t seed = 0;
            for (const auto& knot : curve.getKnotVector()) {
                combineHashes(seed, std::hash<float>{}(knot));
            }
            for (const auto& weights : curve.getWeights()) {
                combineHashes(seed, std::hash<float>{}(weights));
            }
            return seed;
        }
    };

} // namespace pixelforge::management
