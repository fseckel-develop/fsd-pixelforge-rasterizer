#pragma once
#include "InstanceManager.h"
#include "../Geometry/Curves/BezierCurve.h"
#include "../Geometry/Curves/BSpline.h"
#include "../Geometry/Curves/HermiteSpline.h"
#include "../Geometry/Curves/NURBS.h"


// TODO: Including CurveManager in all Curve-using logic
class CurveManager final : public InstanceManager<Curve, CurveManager> {
public:
    static size_t Hash(const Curve& curve) {
        size_t seed = typeid(curve).hash_code();
        CombineHashes(seed, hash<int>{}(curve.GetType()));
        CombineHashes(seed, hash<int>{}(curve.GetForm()));
        for (const auto& controlPoint : curve.GetControlPoints()) {
            CombineHashes(seed, hash<vec3>{}(controlPoint));
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

    static bool Equals(const Curve& left, const Curve& right) {
        if (typeid(left) != typeid(right)) return false;
        if (left.GetType() != right.GetType()) return false;
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
    static size_t HashCurve(const BezierCurve& curve) {
        size_t seed = 0;
        for (const auto& derivativePoint : curve.GetDerivativePoints(0)) {
            CombineHashes(seed, hash<vec3>{}(derivativePoint));
        }
        for (const auto& derivativePoint : curve.GetDerivativePoints(1)) {
            CombineHashes(seed, hash<vec3>{}(derivativePoint));
        }
        return seed;
    }

    static size_t HashCurve(const BSpline& curve) {
        size_t seed = 0;
        for (const auto& knot : curve.GetKnotVector()) {
            CombineHashes(seed, hash<float>{}(knot));
        }
        return seed;
    }

    static size_t HashCurve(const HermiteSpline& curve) {
        size_t seed = 0;
        for (const auto& tangent : curve.GetTangents()) {
            CombineHashes(seed, hash<vec3>{}(tangent));
        }
        return seed;
    }

    static size_t HashCurve(const NURBS& curve) {
        size_t seed = 0;
        for (const auto& knot : curve.GetKnotVector()) {
            CombineHashes(seed, hash<float>{}(knot));
        }
        for (const auto& weights : curve.GetWeights()) {
            CombineHashes(seed, hash<float>{}(weights));
        }
        return seed;
    }
};
