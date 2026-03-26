#include <doctest.h>
#include <managers/curve_manager.hpp>
#include <pixelforge/geometry/curves/bezier_curve.hpp>
#include <pixelforge/geometry/curves/bspline.hpp>
#include <pixelforge/geometry/curves/hermite_spline.hpp>
#include <pixelforge/geometry/curves/nurbs.hpp>
#include <GLM/glm.hpp>
#include <memory>
#include <vector>

using pixelforge::geometry::BezierCurve;
using pixelforge::geometry::BSpline;
using pixelforge::geometry::Curve;
using pixelforge::geometry::HermiteSpline;
using pixelforge::geometry::NURBS;
using pixelforge::management::CurveManager;


namespace {

    std::vector<glm::vec3> basePoints() {
        return {
            {0.0f, 0.0f, 0.0f},
            {1.0f, 2.0f, 0.0f},
            {3.0f, 2.0f, 0.0f},
            {4.0f, 0.0f, 0.0f}
        };
    }

    std::vector<glm::vec3> altPoints() {
        return {
            {0.0f, 0.0f, 0.0f},
            {2.0f, 2.0f, 0.0f},
            {3.0f, 2.0f, 0.0f},
            {4.0f, 0.0f, 0.0f}
        };
    }

    std::vector<glm::vec3> hermiteTangentsA() {
        return {
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {1.0f, -1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f}
        };
    }

    std::vector<glm::vec3> hermiteTangentsB() {
        return {
            {2.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {1.0f, -1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f}
        };
    }

    struct CurveManagerGuard {
        CurveManagerGuard() { CurveManager::clear(); }
        ~CurveManagerGuard() { CurveManager::clear(); }
    };

} // namespace


TEST_CASE("CurveManager returns same pointer for same curve instance") {
    CurveManagerGuard guard;

    const auto curve = std::make_shared<BezierCurve>(basePoints());

    const auto managedA = CurveManager::getOrCreate(curve);
    const auto managedB = CurveManager::getOrCreate(curve);

    CHECK(managedA == curve);
    CHECK(managedB == curve);
    CHECK(managedA == managedB);
}

TEST_CASE("CurveManager canonicalizes equivalent Bezier curves") {
    CurveManagerGuard guard;

    const auto curveA = std::make_shared<BezierCurve>(basePoints());
    const auto curveB = std::make_shared<BezierCurve>(basePoints());

    REQUIRE(curveA != curveB);

    const auto managedA = CurveManager::getOrCreate(curveA);
    const auto managedB = CurveManager::getOrCreate(curveB);

    CHECK(managedA == managedB);
}

TEST_CASE("CurveManager distinguishes Bezier curves with different control points") {
    CurveManagerGuard guard;

    const auto curveA = std::make_shared<BezierCurve>(basePoints());
    const auto curveB = std::make_shared<BezierCurve>(altPoints());

    const auto managedA = CurveManager::getOrCreate(curveA);
    const auto managedB = CurveManager::getOrCreate(curveB);

    CHECK(managedA != managedB);
}

TEST_CASE("CurveManager canonicalizes equivalent BSplines") {
    CurveManagerGuard guard;

    const auto curveA = std::make_shared<BSpline>(basePoints(), 2, Curve::OPEN);
    const auto curveB = std::make_shared<BSpline>(basePoints(), 2, Curve::OPEN);

    const auto managedA = CurveManager::getOrCreate(curveA);
    const auto managedB = CurveManager::getOrCreate(curveB);

    CHECK(managedA == managedB);
}

TEST_CASE("CurveManager distinguishes BSplines with different degree or resulting knot vector") {
    CurveManagerGuard guard;

    const auto curveA = std::make_shared<BSpline>(basePoints(), 2, Curve::OPEN);
    const auto curveB = std::make_shared<BSpline>(basePoints(), 3, Curve::OPEN);

    const auto managedA = CurveManager::getOrCreate(curveA);
    const auto managedB = CurveManager::getOrCreate(curveB);

    CHECK(managedA != managedB);
}

TEST_CASE("CurveManager distinguishes BSplines with different form") {
    CurveManagerGuard guard;

    const auto curveA = std::make_shared<BSpline>(basePoints(), 2, Curve::OPEN);
    const auto curveB = std::make_shared<BSpline>(basePoints(), 2, Curve::LOOP);

    const auto managedA = CurveManager::getOrCreate(curveA);
    const auto managedB = CurveManager::getOrCreate(curveB);

    CHECK(managedA != managedB);
}

TEST_CASE("CurveManager canonicalizes equivalent Hermite splines") {
    CurveManagerGuard guard;

    auto tangentsA = hermiteTangentsA();
    auto tangentsB = hermiteTangentsA();

    const auto curveA = std::make_shared<HermiteSpline>(basePoints(), tangentsA, Curve::OPEN);
    const auto curveB = std::make_shared<HermiteSpline>(basePoints(), tangentsB, Curve::OPEN);

    const auto managedA = CurveManager::getOrCreate(curveA);
    const auto managedB = CurveManager::getOrCreate(curveB);

    CHECK(managedA == managedB);
}

TEST_CASE("CurveManager distinguishes Hermite splines with different tangents") {
    CurveManagerGuard guard;

    auto tangentsA = hermiteTangentsA();
    auto tangentsB = hermiteTangentsB();

    const auto curveA = std::make_shared<HermiteSpline>(basePoints(), tangentsA, Curve::OPEN);
    const auto curveB = std::make_shared<HermiteSpline>(basePoints(), tangentsB, Curve::OPEN);

    const auto managedA = CurveManager::getOrCreate(curveA);
    const auto managedB = CurveManager::getOrCreate(curveB);

    CHECK(managedA != managedB);
}

TEST_CASE("CurveManager canonicalizes equivalent NURBS curves") {
    CurveManagerGuard guard;

    const std::vector weightsA = {1.0f, 2.0f, 3.0f, 4.0f};
    const std::vector weightsB = {1.0f, 2.0f, 3.0f, 4.0f};

    const auto curveA = std::make_shared<NURBS>(basePoints(), weightsA, 2, Curve::OPEN);
    const auto curveB = std::make_shared<NURBS>(basePoints(), weightsB, 2, Curve::OPEN);

    const auto managedA = CurveManager::getOrCreate(curveA);
    const auto managedB = CurveManager::getOrCreate(curveB);

    CHECK(managedA == managedB);
}

TEST_CASE("CurveManager should distinguish NURBS curves with different weights") {
    CurveManagerGuard guard;

    const std::vector weightsA = {1.0f, 1.0f, 1.0f, 1.0f};
    const std::vector weightsB = {1.0f, 2.0f, 1.0f, 1.0f};

    const auto curveA = std::make_shared<NURBS>(basePoints(), weightsA, 2, Curve::OPEN);
    const auto curveB = std::make_shared<NURBS>(basePoints(), weightsB, 2, Curve::OPEN);

    const auto managedA = CurveManager::getOrCreate(curveA);
    const auto managedB = CurveManager::getOrCreate(curveB);

    CHECK(managedA != managedB);
}

TEST_CASE("CurveManager distinguishes different curve types with same control points") {
    CurveManagerGuard guard;

    auto tangents = hermiteTangentsA();

    const auto bezier = std::make_shared<BezierCurve>(basePoints());
    const auto hermite = std::make_shared<HermiteSpline>(basePoints(), tangents, Curve::OPEN);

    const auto managedBezier = CurveManager::getOrCreate(bezier);
    const auto managedHermite = CurveManager::getOrCreate(hermite);

    CHECK(managedBezier != managedHermite);
}
