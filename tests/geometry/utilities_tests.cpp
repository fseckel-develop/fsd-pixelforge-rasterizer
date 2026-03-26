#include <doctest.h>
#include <pixelforge/geometry/utilities.hpp>
#include <GLM/glm.hpp>
#include <vector>


namespace {

constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
    }

    void checkVerticesApprox(const std::vector<glm::vec3>& actual, const std::vector<glm::vec3>& expected) {
        REQUIRE(actual.size() == expected.size());
        for (std::size_t i = 0; i < actual.size(); ++i) {
            checkVec3Approx(actual[i], expected[i]);
        }
    }

    std::vector<glm::vec3> makeVertices() {
        return {
            {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f}
        };
    }

} // namespace


TEST_CASE("geometry utilities translate mutates vertices in place") {
    auto vertices = makeVertices();

    pixelforge::geometry::utilities::translate(vertices, glm::vec3(1.0f, 2.0f, 3.0f));

    checkVerticesApprox(vertices, {
        {2.0f, 2.0f, 3.0f},
        {1.0f, 3.0f, 3.0f},
        {1.0f, 2.0f, 4.0f}
    });
}

TEST_CASE("geometry utilities scale mutates vertices in place") {
    auto vertices = makeVertices();

    pixelforge::geometry::utilities::scale(vertices, glm::vec3(2.0f, 3.0f, 4.0f));

    checkVerticesApprox(vertices, {
        {2.0f, 0.0f, 0.0f},
        {0.0f, 3.0f, 0.0f},
        {0.0f, 0.0f, 4.0f}
    });
}

TEST_CASE("geometry utilities rotate by axis angle mutates vertices in place") {
    auto vertices = makeVertices();

    pixelforge::geometry::utilities::rotate(vertices, glm::half_pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));

    checkVerticesApprox(vertices, {
        {0.0f, 1.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}
    });
}

TEST_CASE("geometry utilities rotate by quaternion mutates vertices in place") {
    auto vertices = makeVertices();
    const glm::quat rotation = glm::angleAxis(glm::half_pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));

    pixelforge::geometry::utilities::rotate(vertices, rotation);

    checkVerticesApprox(vertices, {
        {0.0f, 1.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}
    });
}

TEST_CASE("geometry utilities getTranslatedCopy returns translated copy without mutating original") {
    const auto original = makeVertices();

    const auto translated = pixelforge::geometry::utilities::getTranslatedCopy(
        original, glm::vec3(1.0f, 2.0f, 3.0f)
    );

    checkVerticesApprox(original, makeVertices());
    checkVerticesApprox(translated, {
        {2.0f, 2.0f, 3.0f},
        {1.0f, 3.0f, 3.0f},
        {1.0f, 2.0f, 4.0f}
    });
}

TEST_CASE("geometry utilities getScaledCopy returns scaled copy without mutating original") {
    const auto original = makeVertices();

    const auto scaled = pixelforge::geometry::utilities::getScaledCopy(
        original, glm::vec3(2.0f, 3.0f, 4.0f)
    );

    checkVerticesApprox(original, makeVertices());
    checkVerticesApprox(scaled, {
        {2.0f, 0.0f, 0.0f},
        {0.0f, 3.0f, 0.0f},
        {0.0f, 0.0f, 4.0f}
    });
}

TEST_CASE("geometry utilities getRotatedCopy by axis angle returns rotated copy without mutating original") {
    const auto original = makeVertices();

    const auto rotated = pixelforge::geometry::utilities::getRotatedCopy(
        original, glm::half_pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f)
    );

    checkVerticesApprox(original, makeVertices());
    checkVerticesApprox(rotated, {
        {0.0f, 1.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}
    });
}

TEST_CASE("geometry utilities getRotatedCopy by quaternion returns rotated copy without mutating original") {
    const auto original = makeVertices();
    const glm::quat rotation = glm::angleAxis(glm::half_pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));

    const auto rotated = pixelforge::geometry::utilities::getRotatedCopy(original, rotation);

    checkVerticesApprox(original, makeVertices());
    checkVerticesApprox(rotated, {
        {0.0f, 1.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}
    });
}
