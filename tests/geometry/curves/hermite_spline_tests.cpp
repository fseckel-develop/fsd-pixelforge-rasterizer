#include <doctest.h>
#include <pixelforge/geometry/curves/hermite_spline.hpp>
#include <GLM/glm.hpp>
#include <vector>

using pixelforge::geometry::Curve;
using pixelforge::geometry::HermiteSpline;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
    }

} // namespace


TEST_CASE("HermiteSpline stores explicitly provided tangents") {
    const std::vector<glm::vec3> points = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f},
        {3.0f, 1.0f, 0.0f}
    };

    const std::vector<glm::vec3> tangents = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {1.0f, 1.0f, 0.0f}
    };

    const HermiteSpline spline(points, tangents, Curve::OPEN);

    const auto& storedTangents = spline.getTangents();
    REQUIRE(storedTangents.size() == tangents.size());

    checkVec3Approx(storedTangents[0], tangents[0]);
    checkVec3Approx(storedTangents[1], tangents[1]);
    checkVec3Approx(storedTangents[2], tangents[2]);
}

TEST_CASE("HermiteSpline falls back to Catmull-Rom tangents when tangent count mismatches") {
    const std::vector<glm::vec3> points = {
        {0.0f, 0.0f, 0.0f},
        {2.0f, 0.0f, 0.0f},
        {4.0f, 2.0f, 0.0f}
    };

    const std::vector<glm::vec3> tangents = {
        {99.0f, 99.0f, 99.0f}
    };

    const HermiteSpline spline(points, tangents, Curve::OPEN);
    const auto& computedTangents = spline.getTangents();

    REQUIRE(computedTangents.size() == points.size());

    // Current implementation uses wraparound Catmull-Rom tangents even for OPEN curves:
    // T0 = 0.5 * (P1 - P2)
    // T1 = 0.5 * (P2 - P0)
    // T2 = 0.5 * (P0 - P1)
    checkVec3Approx(computedTangents[0], glm::vec3(-1.0f, -1.0f, 0.0f));
    checkVec3Approx(computedTangents[1], glm::vec3( 2.0f,  1.0f, 0.0f));
    checkVec3Approx(computedTangents[2], glm::vec3(-1.0f,  0.0f, 0.0f));
}

TEST_CASE("HermiteSpline position matches endpoints of first segment with explicit tangents") {
    const std::vector<glm::vec3> points = {
        {0.0f, 0.0f, 0.0f},
        {2.0f, 0.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };

    const std::vector<glm::vec3> tangents = {
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f}
    };

    const HermiteSpline spline(points, tangents, Curve::OPEN);

    // Open spline with 3 points has 2 segments.
    // t = 0.0 -> segment 0, localT = 0.0 => P0
    checkVec3Approx(spline.evaluate(0.0f), points[0]);

    // t = 0.5 -> segmentF = 1.0, segment 1, localT = 0.0 => P1
    checkVec3Approx(spline.evaluate(0.5f), points[1]);
}

TEST_CASE("HermiteSpline reproduces linear segment with matching endpoint tangents") {
    const std::vector<glm::vec3> points = {
        {0.0f, 0.0f, 0.0f},
        {2.0f, 0.0f, 0.0f}
    };

    const std::vector<glm::vec3> tangents = {
        {2.0f, 0.0f, 0.0f},
        {2.0f, 0.0f, 0.0f}
    };

    const HermiteSpline spline(points, tangents, Curve::OPEN);

    checkVec3Approx(spline.evaluate(0.0f),  glm::vec3(0.0f, 0.0f, 0.0f));
    checkVec3Approx(spline.evaluate(0.25f), glm::vec3(0.5f, 0.0f, 0.0f));
    checkVec3Approx(spline.evaluate(0.5f),  glm::vec3(1.0f, 0.0f, 0.0f));
    checkVec3Approx(spline.evaluate(0.75f), glm::vec3(1.5f, 0.0f, 0.0f));
}

TEST_CASE("HermiteSpline velocity matches endpoint tangents on a single segment") {
    const std::vector<glm::vec3> points = {
        {0.0f, 0.0f, 0.0f},
        {2.0f, 0.0f, 0.0f}
    };

    const std::vector<glm::vec3> tangents = {
        {3.0f, 1.0f, 0.0f},
        {-1.0f, 2.0f, 0.0f}
    };

    const HermiteSpline spline(points, tangents, Curve::OPEN);

    checkVec3Approx(spline.evaluate(0.0f, Curve::VELOCITY), tangents[0]);
    checkVec3Approx(spline.evaluate(1.0f, Curve::VELOCITY), tangents[1]);
}

TEST_CASE("HermiteSpline acceleration matches analytic Hermite endpoint values on a single segment") {
    const std::vector<glm::vec3> points = {
        {0.0f, 0.0f, 0.0f},
        {2.0f, 1.0f, 0.0f}
    };

    const std::vector<glm::vec3> tangents = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    };

    const HermiteSpline spline(points, tangents, Curve::OPEN);

    // For a single Hermite segment:
    // P''(0) = -6 p0 - 4 m0 + 6 p1 - 2 m1
    // P''(1) =  6 p0 + 2 m0 - 6 p1 + 4 m1
    checkVec3Approx(
        spline.evaluate(0.0f, Curve::ACCELERATION),
        glm::vec3(8.0f, 4.0f, 0.0f)
    );

    checkVec3Approx(
        spline.evaluate(1.0f, Curve::ACCELERATION),
        glm::vec3(-10.0f, -2.0f, 0.0f)
    );
}

TEST_CASE("HermiteSpline addControlPoint() recomputes Catmull-Rom tangents") {
    HermiteSpline spline({
        {0.0f, 0.0f, 0.0f},
        {2.0f, 0.0f, 0.0f}
    }, Curve::OPEN);

    REQUIRE(spline.getControlPoints().size() == 2);
    REQUIRE(spline.getTangents().size() == 2);

    spline.addControlPoint({4.0f, 2.0f, 0.0f});

    REQUIRE(spline.getControlPoints().size() == 3);
    REQUIRE(spline.getTangents().size() == 3);

    const auto& tangents = spline.getTangents();
    checkVec3Approx(tangents[0], glm::vec3(-1.0f, -1.0f, 0.0f));
    checkVec3Approx(tangents[1], glm::vec3( 2.0f,  1.0f, 0.0f));
    checkVec3Approx(tangents[2], glm::vec3(-1.0f,  0.0f, 0.0f));
}

TEST_CASE("HermiteSpline setTangents() replaces tangent list") {
    HermiteSpline spline({
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {2.0f, 0.0f, 0.0f}
    }, Curve::OPEN);

    const std::vector<glm::vec3> replacementTangents = {
        {5.0f, 0.0f, 0.0f},
        {6.0f, 0.0f, 0.0f},
        {7.0f, 0.0f, 0.0f}
    };

    spline.setTangents(replacementTangents);

    const auto& tangents = spline.getTangents();
    REQUIRE(tangents.size() == replacementTangents.size());

    checkVec3Approx(tangents[0], replacementTangents[0]);
    checkVec3Approx(tangents[1], replacementTangents[1]);
    checkVec3Approx(tangents[2], replacementTangents[2]);
}
