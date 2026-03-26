#include <doctest.h>
#include <pixelforge/geometry/curves/bspline.hpp>
#include <GLM/glm.hpp>
#include <vector>

using pixelforge::geometry::BSpline;
using pixelforge::geometry::Curve;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected, float eps = EPS) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(eps));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(eps));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(eps));
    }

    void checkFloatVectorApprox(const std::vector<float>& actual,
                                const std::vector<float>& expected,
                                float eps = EPS) {
        REQUIRE(actual.size() == expected.size());
        for (std::size_t i = 0; i < actual.size(); ++i) {
            CHECK(actual[i] == doctest::Approx(expected[i]).epsilon(eps));
        }
    }

} // namespace


TEST_CASE("BSpline open knot vector has expected size and clamped form") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {2.0f, 0.0f, 0.0f},
        {3.0f, 0.0f, 0.0f}
    };

    const BSpline spline(controlPoints, 2, Curve::OPEN);

    const auto& knots = spline.getKnotVector();

    // K = 4, degree = 2
    // open knot vector before normalization: [0,0,0,1,2,2,2]
    // normalizer = K - degree = 2
    // expected: [0,0,0,0.5,1,1,1]
    const std::vector<float> expected = {0.0f, 0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f};

    checkFloatVectorApprox(knots, expected);
}

TEST_CASE("BSpline corrects degree when it is too large for the number of control points") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {2.0f, 0.0f, 0.0f}
    };

    const BSpline spline(controlPoints, 10, Curve::OPEN);

    const auto& knots = spline.getKnotVector();

    // Degree should be corrected to controlPoints.size() - 1 = 2
    // K = 3, degree = 2
    // open knot vector before normalization: [0,0,0,1,1,1]
    const std::vector<float> expected = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};

    checkFloatVectorApprox(knots, expected);
}

TEST_CASE("BSpline addControlPoint regenerates knot vector") {
    BSpline spline({
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {2.0f, 0.0f, 0.0f}
    }, 1, Curve::OPEN);

    const auto initialKnots = spline.getKnotVector();

    spline.addControlPoint({3.0f, 0.0f, 0.0f});

    const auto& updatedKnots = spline.getKnotVector();

    REQUIRE(updatedKnots.size() > initialKnots.size());

    // K = 4, degree = 1
    // open knot vector before normalization: [0,0,1,2,3,3]
    // normalizer = 3
    const std::vector<float> expected = {
        0.0f, 0.0f,
        1.0f / 3.0f,
        2.0f / 3.0f,
        1.0f, 1.0f
    };

    checkFloatVectorApprox(updatedKnots, expected);
}

TEST_CASE("BSpline open curve returns first and last control points at the endpoints") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {3.0f, 2.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const BSpline spline(controlPoints, 2, Curve::OPEN);

    checkVec3Approx(spline.evaluate(0.0f), controlPoints.front());
    checkVec3Approx(spline.evaluate(1.0f), controlPoints.back());
}

TEST_CASE("BSpline evaluate clamps parameters for open curves") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 0.0f},
        {2.0f, 0.0f, 0.0f}
    };

    const BSpline spline(controlPoints, 2, Curve::OPEN);

    checkVec3Approx(spline.evaluate(-1.0f), controlPoints.front());
    checkVec3Approx(spline.evaluate(2.0f), controlPoints.back());
}

TEST_CASE("Degree-1 BSpline is piecewise linear on open curve") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {2.0f, 0.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const BSpline spline(controlPoints, 1, Curve::OPEN);

    // Knot vector becomes [0,0,0.5,1,1]
    // For degree 1, the curve should linearly interpolate between
    // P0 -> P1 on [0,0.5] and P1 -> P2 on [0.5,1].
    checkVec3Approx(spline.evaluate(0.0f),  glm::vec3(0.0f, 0.0f, 0.0f));
    checkVec3Approx(spline.evaluate(0.25f), glm::vec3(1.0f, 0.0f, 0.0f));
    checkVec3Approx(spline.evaluate(0.5f),  glm::vec3(2.0f, 0.0f, 0.0f));
    checkVec3Approx(spline.evaluate(0.75f), glm::vec3(3.0f, 0.0f, 0.0f));
    checkVec3Approx(spline.evaluate(1.0f),  glm::vec3(4.0f, 0.0f, 0.0f));
}

TEST_CASE("Degree-1 BSpline has constant velocity magnitude on each linear segment") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {2.0f, 0.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const BSpline spline(controlPoints, 1, Curve::OPEN);

    const glm::vec3 v1 = spline.evaluate(0.25f, Curve::VELOCITY);
    const glm::vec3 v2 = spline.evaluate(0.75f, Curve::VELOCITY);

    checkVec3Approx(v1, glm::vec3(4.0f, 0.0f, 0.0f), 1e-4f);
    checkVec3Approx(v2, glm::vec3(4.0f, 0.0f, 0.0f), 1e-4f);
}

TEST_CASE("Degree-1 BSpline acceleration is zero away from segment boundaries") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {2.0f, 0.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const BSpline spline(controlPoints, 1, Curve::OPEN);

    checkVec3Approx(
        spline.evaluate(0.25f, Curve::ACCELERATION),
        glm::vec3(0.0f, 0.0f, 0.0f),
        1e-4f
    );
    checkVec3Approx(
        spline.evaluate(0.75f, Curve::ACCELERATION),
        glm::vec3(0.0f, 0.0f, 0.0f),
        1e-4f
    );
}

TEST_CASE("BSpline sample returns expected number of points with current implementation") {
    const BSpline spline({
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {2.0f, 0.0f, 0.0f}
    }, 1, Curve::OPEN);

    const auto samples = spline.sample(4, Curve::POSITION);

    REQUIRE(samples.size() == 5);
    checkVec3Approx(samples.front(), glm::vec3(0.0f, 0.0f, 0.0f));
    checkVec3Approx(samples.back(),  glm::vec3(2.0f, 0.0f, 0.0f));
}
