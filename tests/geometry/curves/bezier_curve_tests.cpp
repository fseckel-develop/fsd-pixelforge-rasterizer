#include <doctest.h>
#include <pixelforge/geometry/curves/bezier_curve.hpp>
#include <GLM/glm.hpp>
#include <vector>

using pixelforge::geometry::BezierCurve;
using pixelforge::geometry::Curve;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
    }

} // namespace


TEST_CASE("BezierCurve evaluates to first and last control points at the ends") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {3.0f, 2.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const BezierCurve curve(controlPoints);

    checkVec3Approx(curve.evaluate(0.0f), controlPoints.front());
    checkVec3Approx(curve.evaluate(1.0f), controlPoints.back());
}

TEST_CASE("BezierCurve quadratic midpoint matches known De Casteljau result") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {2.0f, 2.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const BezierCurve curve(controlPoints);

    // Quadratic Bézier at t = 0.5:
    // B(0.5) = 0.25*P0 + 0.5*P1 + 0.25*P2 = (2, 1, 0)
    checkVec3Approx(curve.evaluate(0.5f), glm::vec3(2.0f, 1.0f, 0.0f));
}

TEST_CASE("BezierCurve evaluate() clamps parameters for open curves") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {3.0f, 2.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const BezierCurve curve(controlPoints);

    checkVec3Approx(curve.evaluate(-1.0f), controlPoints.front());
    checkVec3Approx(curve.evaluate(2.0f), controlPoints.back());
}

TEST_CASE("BezierCurve stores first and second derivative control points for cubic curves") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {3.0f, 2.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const BezierCurve curve(controlPoints);

    const auto& firstDerivative = curve.getDerivativePoints(0);
    const auto& secondDerivative = curve.getDerivativePoints(1);

    REQUIRE(firstDerivative.size() == 3);
    REQUIRE(secondDerivative.size() == 2);

    // First derivative control points: n * (P_{i+1} - P_i), n = 3
    checkVec3Approx(firstDerivative[0], glm::vec3(3.0f, 6.0f, 0.0f));
    checkVec3Approx(firstDerivative[1], glm::vec3(6.0f, 0.0f, 0.0f));
    checkVec3Approx(firstDerivative[2], glm::vec3(3.0f, -6.0f, 0.0f));

    // Second derivative control points from the first derivative polygon.
    // Current implementation uses (n-1) * (D1_{i+1} - D1_i), with n-1 = 2
    checkVec3Approx(secondDerivative[0], glm::vec3(6.0f, -12.0f, 0.0f));
    checkVec3Approx(secondDerivative[1], glm::vec3(-6.0f, -12.0f, 0.0f));
}

TEST_CASE("BezierCurve addControlPoint() updates degree and derivative caches") {
    BezierCurve curve({
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 0.0f},
        {2.0f, 0.0f, 0.0f}
    });

    REQUIRE(curve.getControlPoints().size() == 3);
    REQUIRE(curve.getDerivativePoints(0).size() == 2);
    REQUIRE(curve.getDerivativePoints(1).size() == 1);

    curve.addControlPoint({3.0f, -1.0f, 0.0f});

    REQUIRE(curve.getControlPoints().size() == 4);
    REQUIRE(curve.getDerivativePoints(0).size() == 3);
    REQUIRE(curve.getDerivativePoints(1).size() == 2);
}

TEST_CASE("BezierCurve cubic velocity matches analytic derivative at endpoints") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {3.0f, 2.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const BezierCurve curve(controlPoints);

    // For cubic Bézier:
    // B'(0) = 3(P1 - P0)
    // B'(1) = 3(P3 - P2)
    checkVec3Approx(
        curve.evaluate(0.0f, Curve::VELOCITY),
        glm::vec3(3.0f, 6.0f, 0.0f)
    );

    checkVec3Approx(
        curve.evaluate(1.0f, Curve::VELOCITY),
        glm::vec3(3.0f, -6.0f, 0.0f)
    );
}

TEST_CASE("BezierCurve cubic acceleration matches analytic second derivative at endpoints") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {3.0f, 2.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const BezierCurve curve(controlPoints);

    // For cubic Bézier:
    // B''(0) = 6(P2 - 2P1 + P0) = (6, -12, 0)
    // B''(1) = 6(P3 - 2P2 + P1) = (-6, -12, 0)
    checkVec3Approx(
        curve.evaluate(0.0f, Curve::ACCELERATION),
        glm::vec3(6.0f, -12.0f, 0.0f)
    );

    checkVec3Approx(
        curve.evaluate(1.0f, Curve::ACCELERATION),
        glm::vec3(-6.0f, -12.0f, 0.0f)
    );
}

TEST_CASE("BezierCurve sample() returns expected number of points with current implementation") {
    const BezierCurve curve({
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {3.0f, 2.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    });

    const auto samples = curve.sample(4, Curve::POSITION);

    // Current implementation uses:
    // step = (tMax - tMin) / sampleCount
    // while (t <= tMax) push sample
    // so sample(4) over [0,1] produces 5 samples.
    REQUIRE(samples.size() == 5);
    checkVec3Approx(samples.front(), glm::vec3(0.0f, 0.0f, 0.0f));
    checkVec3Approx(samples.back(),  glm::vec3(4.0f, 0.0f, 0.0f));
}
