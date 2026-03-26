#include <doctest.h>
#include <pixelforge/geometry/meshes/cuboid.hpp>
#include <pixelforge/graphics/pipeline/vertex_data.hpp>
#include <GLM/glm.hpp>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <vector>


using pixelforge::geometry::Cuboid;

namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Finite(const glm::vec3& v) {
        CHECK(std::isfinite(v.x));
        CHECK(std::isfinite(v.y));
        CHECK(std::isfinite(v.z));
    }

} // namespace


TEST_CASE("Cuboid generates expected vertex and index counts") {
    const Cuboid cuboid(2.0f, 4.0f, 6.0f);

    const auto& vertexData = cuboid.getVertexData();
    const auto& indices = cuboid.getIndices();

    REQUIRE(vertexData.hasAttribute(pixelforge::graphics::POSITION));

    const auto positions =
        vertexData.getAttribute<glm::vec3>(pixelforge::graphics::POSITION);

    REQUIRE(positions.size() == 8);
    REQUIRE(indices.size() == 36);
    CHECK(cuboid.getVertexCount() == 8);
}

TEST_CASE("Cuboid indices are within vertex range") {
    const Cuboid cuboid(2.0f, 4.0f, 6.0f);

    const auto positions =
        cuboid.getVertexData().getAttribute<glm::vec3>(pixelforge::graphics::POSITION);
    const auto& indices = cuboid.getIndices();

    REQUIRE(positions.size() == 8);
    REQUIRE(indices.size() == 36);

    for (const auto index : indices) {
        CHECK(static_cast<std::size_t>(index) < positions.size());
    }
}

TEST_CASE("Cuboid positions are finite") {
    const Cuboid cuboid(2.0f, 4.0f, 6.0f);

    const auto positions =
        cuboid.getVertexData().getAttribute<glm::vec3>(pixelforge::graphics::POSITION);

    REQUIRE(positions.size() == 8);

    for (const auto& position : positions) {
        checkVec3Finite(position);
    }
}

TEST_CASE("Cuboid bounds match half extents") {
    constexpr float width = 2.0f;
    constexpr float height = 4.0f;
    constexpr float depth = 6.0f;

    const Cuboid cuboid(width, height, depth);

    const auto positions =
        cuboid.getVertexData().getAttribute<glm::vec3>(pixelforge::graphics::POSITION);

    REQUIRE(positions.size() == 8);

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();

    for (const auto& position : positions) {
        minX = std::min(minX, position.x);
        maxX = std::max(maxX, position.x);
        minY = std::min(minY, position.y);
        maxY = std::max(maxY, position.y);
        minZ = std::min(minZ, position.z);
        maxZ = std::max(maxZ, position.z);
    }

    CHECK(minX == doctest::Approx(-width / 2.0f).epsilon(EPS));
    CHECK(maxX == doctest::Approx( width / 2.0f).epsilon(EPS));
    CHECK(minY == doctest::Approx(-height / 2.0f).epsilon(EPS));
    CHECK(maxY == doctest::Approx( height / 2.0f).epsilon(EPS));
    CHECK(minZ == doctest::Approx(-depth / 2.0f).epsilon(EPS));
    CHECK(maxZ == doctest::Approx( depth / 2.0f).epsilon(EPS));
}

TEST_CASE("Cuboid is centered at the origin") {
    const Cuboid cuboid(2.0f, 4.0f, 6.0f);

    const auto positions =
        cuboid.getVertexData().getAttribute<glm::vec3>(pixelforge::graphics::POSITION);

    REQUIRE(positions.size() == 8);

    glm::vec3 centroid(0.0f);
    for (const auto& position : positions) {
        centroid += position;
    }
    centroid /= static_cast<float>(positions.size());

    CHECK(centroid.x == doctest::Approx(0.0f).epsilon(EPS));
    CHECK(centroid.y == doctest::Approx(0.0f).epsilon(EPS));
    CHECK(centroid.z == doctest::Approx(0.0f).epsilon(EPS));
}

TEST_CASE("Cuboid has all eight expected corner positions") {
    constexpr float width = 2.0f;
    constexpr float height = 4.0f;
    constexpr float depth = 6.0f;

    const Cuboid cuboid(width, height, depth);

    const auto positions =
        cuboid.getVertexData().getAttribute<glm::vec3>(pixelforge::graphics::POSITION);

    REQUIRE(positions.size() == 8);

    const float x = width / 2.0f;
    const float y = height / 2.0f;
    const float z = depth / 2.0f;

    const std::vector<glm::vec3> expectedCorners = {
        {-x, -y, -z},
        {-x, -y,  z},
        {-x,  y, -z},
        {-x,  y,  z},
        { x, -y, -z},
        { x, -y,  z},
        { x,  y, -z},
        { x,  y,  z}
    };

    for (const auto& expected : expectedCorners) {
        bool found = false;
        for (const auto& actual : positions) {
            if (glm::length(actual - expected) <= EPS) {
                found = true;
                break;
            }
        }
        CHECK(found);
    }
}
