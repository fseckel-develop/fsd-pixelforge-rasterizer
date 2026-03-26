#include <doctest.h>
#include <pixelforge/geometry/meshes/sphere.hpp>
#include <pixelforge/graphics/pipeline/vertex_data.hpp>
#include <GLM/glm.hpp>
#include <cmath>
#include <vector>

using pixelforge::geometry::Sphere;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
    }

    void checkUV(const glm::vec2& uv) {
        CHECK(std::isfinite(uv.x));
        CHECK(std::isfinite(uv.y));
        CHECK(uv.x >= -EPS);
        CHECK(uv.y >= -EPS);
        CHECK(uv.x <= 1.0f + EPS);
        CHECK(uv.y <= 1.0f + EPS);
    }

    void checkVec3Finite(const glm::vec3& v) {
        CHECK(std::isfinite(v.x));
        CHECK(std::isfinite(v.y));
        CHECK(std::isfinite(v.z));
    }

} // namespace


TEST_CASE("Sphere generates expected attribute and index counts") {
    constexpr float radius = 2.0f;
    constexpr unsigned int stacks = 4;
    constexpr unsigned int sectors = 8;

    const Sphere sphere(radius, stacks, sectors);

    const auto& vertexData = sphere.getVertexData();
    const auto& indices = sphere.getIndices();

    REQUIRE(vertexData.hasAttribute(pixelforge::graphics::POSITION));
    REQUIRE(vertexData.hasAttribute(pixelforge::graphics::NORMAL));
    REQUIRE(vertexData.hasAttribute(pixelforge::graphics::TEXTURE));

    const auto& positions = vertexData.getAttribute<glm::vec3>(pixelforge::graphics::POSITION);
    const auto& normals = vertexData.getAttribute<glm::vec3>(pixelforge::graphics::NORMAL);
    const auto& texCoords = vertexData.getAttribute<glm::vec2>(pixelforge::graphics::TEXTURE);

    constexpr auto expectedVertexCount = static_cast<std::size_t>((stacks + 1) * (sectors + 1));
    constexpr auto expectedIndexCount = static_cast<std::size_t>(stacks * sectors * 6);

    REQUIRE(positions.size() == expectedVertexCount);
    REQUIRE(normals.size() == expectedVertexCount);
    REQUIRE(texCoords.size() == expectedVertexCount);
    REQUIRE(indices.size() == expectedIndexCount);

    CHECK(sphere.getVertexCount() == static_cast<GLsizei>(expectedVertexCount));
}

TEST_CASE("Sphere indices are in range") {
    constexpr float radius = 1.0f;
    constexpr unsigned int stacks = 6;
    constexpr unsigned int sectors = 10;

    const Sphere sphere(radius, stacks, sectors);

    const auto positions = sphere.getVertexData().getAttribute<glm::vec3>(pixelforge::graphics::POSITION);
    const auto& indices = sphere.getIndices();

    REQUIRE_FALSE(positions.empty());
    REQUIRE_FALSE(indices.empty());

    for (const auto index : indices) {
        CHECK(static_cast<std::size_t>(index) < positions.size());
    }
}

TEST_CASE("Sphere positions lie on requested radius") {
    constexpr float radius = 3.5f;
    constexpr unsigned int stacks = 5;
    constexpr unsigned int sectors = 9;

    const Sphere sphere(radius, stacks, sectors);

    const auto positions = sphere.getVertexData().getAttribute<glm::vec3>(pixelforge::graphics::POSITION);
    REQUIRE_FALSE(positions.empty());

    for (const auto& position : positions) {
        checkVec3Finite(position);
        const float distance = glm::length(position);
        CHECK(distance == doctest::Approx(radius).epsilon(1e-4f));
    }
}

TEST_CASE("Sphere normals are unit length and match normalized positions") {
    constexpr float radius = 2.25f;
    constexpr unsigned int stacks = 7;
    constexpr unsigned int sectors = 11;

    const Sphere sphere(radius, stacks, sectors);

    const auto positions = sphere.getVertexData().getAttribute<glm::vec3>(pixelforge::graphics::POSITION);
    const auto normals = sphere.getVertexData().getAttribute<glm::vec3>(pixelforge::graphics::NORMAL);

    REQUIRE(positions.size() == normals.size());

    for (std::size_t i = 0; i < positions.size(); ++i) {
        const auto& position = positions[i];
        const auto& normal = normals[i];

        checkVec3Finite(normal);

        CHECK(glm::length(normal) == doctest::Approx(1.0f).epsilon(1e-4f));

        const glm::vec3 expectedNormal = glm::normalize(position);
        checkVec3Approx(normal, expectedNormal);
    }
}

TEST_CASE("Sphere texture coordinates stay within unit square") {
    constexpr float radius = 1.0f;
    constexpr unsigned int stacks = 4;
    constexpr unsigned int sectors = 8;

    const Sphere sphere(radius, stacks, sectors);

    const auto texCoords = sphere.getVertexData().getAttribute<glm::vec2>(pixelforge::graphics::TEXTURE);
    REQUIRE_FALSE(texCoords.empty());

    for (const auto& uv : texCoords) {
        checkUV(uv);
    }
}

TEST_CASE("Sphere contains north and south pole positions") {
    constexpr float radius = 2.0f;
    constexpr unsigned int stacks = 4;
    constexpr unsigned int sectors = 8;

    const Sphere sphere(radius, stacks, sectors);

    const auto positions = sphere.getVertexData().getAttribute<glm::vec3>(pixelforge::graphics::POSITION);
    REQUIRE_FALSE(positions.empty());

    bool foundNorthPole = false;
    bool foundSouthPole = false;

    for (const auto& position : positions) {
        if (glm::length(position - glm::vec3(0.0f, radius, 0.0f)) < 1e-4f) {
            foundNorthPole = true;
        }
        if (glm::length(position - glm::vec3(0.0f, -radius, 0.0f)) < 1e-4f) {
            foundSouthPole = true;
        }
    }

    CHECK(foundNorthPole);
    CHECK(foundSouthPole);
}
