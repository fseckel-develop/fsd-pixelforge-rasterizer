#include <doctest.h>
#include <pixelforge/geometry/curves/lagrange_curve.hpp>
#include <GLM/glm.hpp>
#include <vector>

using pixelforge::geometry::Curve;
using pixelforge::geometry::LagrangeCurve;


namespace {

constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected, float eps = EPS) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(eps));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(eps));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(eps));
    }

} // namespace


TEST_CASE("LagrangeCurve evaluates to first and last control points at the ends") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {3.0f, 1.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const LagrangeCurve curve(controlPoints);

    checkVec3Approx(curve.evaluate(0.0f), controlPoints.front());
    checkVec3Approx(curve.evaluate(1.0f), controlPoints.back());
}

TEST_CASE("LagrangeCurve interpolates all control points at uniform knot parameters") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {2.0f, 1.0f, 0.0f},
        {4.0f, 3.0f, 0.0f},
        {6.0f, 0.0f, 0.0f}
    };

    const LagrangeCurve curve(controlPoints);
    const int n = static_cast<int>(controlPoints.size());

    for (int i = 0; i < n; ++i) {
        const float t = static_cast<float>(i) / static_cast<float>(n - 1);
        checkVec3Approx(curve.evaluate(t), controlPoints[static_cast<std::size_t>(i)]);
    }
}

TEST_CASE("LagrangeCurve evaluate clamps parameters for open curves") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {2.0f, 1.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const LagrangeCurve curve(controlPoints);

    checkVec3Approx(curve.evaluate(-1.0f), controlPoints.front());
    checkVec3Approx(curve.evaluate(2.0f), controlPoints.back());
}

TEST_CASE("LagrangeCurve quadratic midpoint matches known interpolated value") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {2.0f, 0.0f, 0.0f}
    };

    const LagrangeCurve curve(controlPoints);

    // With 3 control points, knots are at t = 0, 0.5, 1.
    // The curve must interpolate the middle control point at t = 0.5.
    checkVec3Approx(curve.evaluate(0.5f), glm::vec3(1.0f, 2.0f, 0.0f));
}

TEST_CASE("LagrangeCurve linear case is a straight line with constant velocity") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {2.0f, 4.0f, 0.0f}
    };

    const LagrangeCurve curve(controlPoints);

    checkVec3Approx(curve.evaluate(0.0f),  glm::vec3(0.0f, 0.0f, 0.0f));
    checkVec3Approx(curve.evaluate(0.25f), glm::vec3(0.5f, 1.0f, 0.0f));
    checkVec3Approx(curve.evaluate(0.5f),  glm::vec3(1.0f, 2.0f, 0.0f));
    checkVec3Approx(curve.evaluate(0.75f), glm::vec3(1.5f, 3.0f, 0.0f));
    checkVec3Approx(curve.evaluate(1.0f),  glm::vec3(2.0f, 4.0f, 0.0f));

    checkVec3Approx(
        curve.evaluate(0.1f, Curve::VELOCITY),
        glm::vec3(2.0f, 4.0f, 0.0f)
    );
    checkVec3Approx(
        curve.evaluate(0.9f, Curve::VELOCITY),
        glm::vec3(2.0f, 4.0f, 0.0f)
    );

    checkVec3Approx(
        curve.evaluate(0.5f, Curve::ACCELERATION),
        glm::vec3(0.0f, 0.0f, 0.0f)
    );
}

TEST_CASE("LagrangeCurve quadratic case has constant acceleration") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {2.0f, 0.0f, 0.0f}
    };

    const LagrangeCurve curve(controlPoints);

    const glm::vec3 a0 = curve.evaluate(0.25f, Curve::ACCELERATION);
    const glm::vec3 a1 = curve.evaluate(0.75f, Curve::ACCELERATION);

    checkVec3Approx(a0, a1, 1e-4f);
}

TEST_CASE("LagrangeCurve sample returns expected number of points with current implementation") {
    const LagrangeCurve curve({
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {2.0f, 0.0f, 0.0f}
    });

    const auto samples = curve.sample(4, Curve::POSITION);

    // Current Curve::sample implementation includes both ends for sample(4),
    // so it produces 5 samples over [0, 1].
    REQUIRE(samples.size() == 5);
    checkVec3Approx(samples.front(), glm::vec3(0.0f, 0.0f, 0.0f));
    checkVec3Approx(samples.back(),  glm::vec3(2.0f, 0.0f, 0.0f));
}
