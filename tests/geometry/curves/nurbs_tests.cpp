#include <doctest.h>
#include <pixelforge/geometry/curves/bspline.hpp>
#include <pixelforge/geometry/curves/nurbs.hpp>
#include <GLM/glm.hpp>
#include <vector>

using pixelforge::geometry::BSpline;
using pixelforge::geometry::Curve;
using pixelforge::geometry::NURBS;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected, float eps = EPS) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(eps));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(eps));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(eps));
    }

    void checkFloatVectorApprox(const std::vector<float>& actual, const std::vector<float>& expected) {
        REQUIRE(actual.size() == expected.size());
        for (std::size_t i = 0; i < actual.size(); ++i) {
            CHECK(actual[i] == doctest::Approx(expected[i]).epsilon(EPS));
        }
    }

} // namespace


TEST_CASE("NURBS stores provided weights when count matches control points") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {3.0f, 2.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const std::vector<float> weights = {1.0f, 2.0f, 3.0f, 4.0f};

    const NURBS curve(controlPoints, weights, 2, Curve::OPEN);

    checkFloatVectorApprox(curve.getWeights(), weights);
}

TEST_CASE("NURBS falls back to unit weights when weight count mismatches") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {3.0f, 2.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const std::vector weights = {5.0f, 6.0f};

    const NURBS curve(controlPoints, weights, 2, Curve::OPEN);

    const std::vector expected(controlPoints.size(), 1.0f);
    checkFloatVectorApprox(curve.getWeights(), expected);
}

TEST_CASE("NURBS with unit weights matches BSpline positions") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {3.0f, 2.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const std::vector weights(controlPoints.size(), 1.0f);

    const BSpline bspline(controlPoints, 2, Curve::OPEN);
    const NURBS nurbs(controlPoints, weights, 2, Curve::OPEN);

    const std::vector params = {0.0f, 0.2f, 0.5f, 0.8f, 1.0f};

    for (const float t : params) {
        checkVec3Approx(nurbs.evaluate(t), bspline.evaluate(t), 1e-4f);
    }
}

TEST_CASE("NURBS with unit weights matches BSpline velocity") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {3.0f, 2.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const std::vector weights(controlPoints.size(), 1.0f);

    const BSpline bspline(controlPoints, 2, Curve::OPEN);
    const NURBS nurbs(controlPoints, weights, 2, Curve::OPEN);

    const std::vector params = {0.2f, 0.5f, 0.8f};

    for (const float t : params) {
        checkVec3Approx(
            nurbs.evaluate(t, Curve::VELOCITY),
            bspline.evaluate(t, Curve::VELOCITY),
            1e-4f
        );
    }
}

TEST_CASE("NURBS open curve returns first and last control points at endpoints") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {3.0f, 2.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const std::vector weights = {1.0f, 2.0f, 0.5f, 3.0f};

    const NURBS curve(controlPoints, weights, 2, Curve::OPEN);

    checkVec3Approx(curve.evaluate(0.0f), controlPoints.front());
    checkVec3Approx(curve.evaluate(1.0f), controlPoints.back());
}

TEST_CASE("NURBS evaluate clamps parameters for open curves") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {2.0f, 1.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const std::vector weights = {1.0f, 3.0f, 1.0f};

    const NURBS curve(controlPoints, weights, 2, Curve::OPEN);

    checkVec3Approx(curve.evaluate(-1.0f), controlPoints.front());
    checkVec3Approx(curve.evaluate(2.0f), controlPoints.back());
}

TEST_CASE("NURBS non-unit middle weight influences curve shape") {
    const std::vector<glm::vec3> controlPoints = {
        {0.0f, 0.0f, 0.0f},
        {2.0f, 2.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const NURBS unitWeights(controlPoints, {1.0f, 1.0f, 1.0f}, 2, Curve::OPEN);
    const NURBS heavyMiddle(controlPoints, {1.0f, 10.0f, 1.0f}, 2, Curve::OPEN);

    const glm::vec3 pUnit = unitWeights.evaluate(0.5f);
    const glm::vec3 pHeavy = heavyMiddle.evaluate(0.5f);

    CHECK(pHeavy.y > pUnit.y);
}

TEST_CASE("NURBS addControlPoint adds default unit weight") {
    NURBS curve({
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 0.0f},
        {2.0f, 0.0f, 0.0f}
    }, {
        1.0f, 2.0f, 3.0f
    }, 2, Curve::OPEN);

    curve.addControlPoint({3.0f, -1.0f, 0.0f});

    const auto& weights = curve.getWeights();
    REQUIRE(weights.size() == 4);
    CHECK(weights.back() == doctest::Approx(1.0f));
}

TEST_CASE("NURBS addControlPointWithWeight appends provided weight") {
    NURBS curve({
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 0.0f},
        {2.0f, 0.0f, 0.0f}
    }, {
        1.0f, 2.0f, 3.0f
    }, 2, Curve::OPEN);

    curve.addControlPointWithWeight({3.0f, -1.0f, 0.0f}, 7.5f);

    const auto& weights = curve.getWeights();
    REQUIRE(weights.size() == 4);
    CHECK(weights.back() == doctest::Approx(7.5f));
}

TEST_CASE("NURBS sample returns expected number of points with current implementation") {
    const NURBS curve({
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {2.0f, 0.0f, 0.0f}
    }, {
        1.0f, 1.0f, 1.0f
    }, 2, Curve::OPEN);

    const auto samples = curve.sample(4, Curve::POSITION);

    REQUIRE(samples.size() == 5);
    checkVec3Approx(samples.front(), glm::vec3(0.0f, 0.0f, 0.0f));
    checkVec3Approx(samples.back(),  glm::vec3(2.0f, 0.0f, 0.0f));
}
