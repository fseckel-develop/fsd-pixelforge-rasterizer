#include <doctest.h>
#include <pixelforge/geometry/meshes/torus.hpp>
#include <pixelforge/graphics/pipeline/vertex_data.hpp>
#include <GLM/glm.hpp>
#include <cmath>
#include <cstddef>
#include <limits>
#include <vector>


using pixelforge::geometry::Torus;

namespace {

    constexpr float EPS = 1e-4f;

    void checkVec3Finite(const glm::vec3& v) {
        CHECK(std::isfinite(v.x));
        CHECK(std::isfinite(v.y));
        CHECK(std::isfinite(v.z));
    }

} // namespace


TEST_CASE("Torus generates expected vertex and index counts") {
    constexpr float innerRadius = 2.0f;
    constexpr float torusRadius = 0.5f;
    constexpr unsigned int stacks = 6;
    constexpr unsigned int sectors = 8;

    const Torus torus(innerRadius, torusRadius, stacks, sectors);

    const auto& vertexData = torus.getVertexData();
    const auto& indices = torus.getIndices();

    REQUIRE(vertexData.hasAttribute(pixelforge::graphics::POSITION));

    const auto positions = vertexData.getAttribute<glm::vec3>(pixelforge::graphics::POSITION);

    const auto expectedVertexCount =
        static_cast<std::size_t>((stacks + 1) * (sectors + 1));
    const auto expectedIndexCount =
        static_cast<std::size_t>(stacks * sectors * 6);

    REQUIRE(positions.size() == expectedVertexCount);
    REQUIRE(indices.size() == expectedIndexCount);
    CHECK(torus.getVertexCount() == static_cast<GLsizei>(expectedVertexCount));
}

TEST_CASE("Torus indices are within vertex range") {
    constexpr float innerRadius = 1.5f;
    constexpr float torusRadius = 0.75f;
    constexpr unsigned int stacks = 5;
    constexpr unsigned int sectors = 7;

    const Torus torus(innerRadius, torusRadius, stacks, sectors);

    const auto positions =
        torus.getVertexData().getAttribute<glm::vec3>(pixelforge::graphics::POSITION);
    const auto& indices = torus.getIndices();

    REQUIRE_FALSE(positions.empty());
    REQUIRE_FALSE(indices.empty());

    for (const auto index : indices) {
        CHECK(static_cast<std::size_t>(index) < positions.size());
    }
}

TEST_CASE("Torus positions are finite") {
    constexpr float innerRadius = 2.0f;
    constexpr float torusRadius = 1.0f;
    constexpr unsigned int stacks = 8;
    constexpr unsigned int sectors = 10;

    const Torus torus(innerRadius, torusRadius, stacks, sectors);

    const auto positions =
        torus.getVertexData().getAttribute<glm::vec3>(pixelforge::graphics::POSITION);

    REQUIRE_FALSE(positions.empty());

    for (const auto& position : positions) {
        checkVec3Finite(position);
    }
}

TEST_CASE("Torus major radius stays within expected radial bounds in XY plane") {
    constexpr float innerRadius = 3.0f;
    constexpr float torusRadius = 1.0f;
    constexpr unsigned int stacks = 12;
    constexpr unsigned int sectors = 16;

    const Torus torus(innerRadius, torusRadius, stacks, sectors);

    const auto positions =
        torus.getVertexData().getAttribute<glm::vec3>(pixelforge::graphics::POSITION);

    REQUIRE_FALSE(positions.empty());

    float minXYRadius = std::numeric_limits<float>::max();
    float maxXYRadius = 0.0f;

    for (const auto& position : positions) {
        const float xyRadius = std::sqrt(position.x * position.x + position.y * position.y);
        minXYRadius = std::min(minXYRadius, xyRadius);
        maxXYRadius = std::max(maxXYRadius, xyRadius);
    }

    // Based on the current implementation:
    // radiusZ = innerRadius + torusRadius - cos(stackAngle) * torusRadius
    // so XY radius ranges from innerRadius to innerRadius + 2 * torusRadius
    CHECK(minXYRadius == doctest::Approx(innerRadius).epsilon(EPS));
    CHECK(maxXYRadius == doctest::Approx(innerRadius + 2.0f * torusRadius).epsilon(EPS));
}

TEST_CASE("Torus z extent matches torus radius") {
    constexpr float innerRadius = 2.5f;
    constexpr float torusRadius = 0.75f;
    constexpr unsigned int stacks = 16;
    constexpr unsigned int sectors = 12;

    const Torus torus(innerRadius, torusRadius, stacks, sectors);

    const auto positions =
        torus.getVertexData().getAttribute<glm::vec3>(pixelforge::graphics::POSITION);

    REQUIRE_FALSE(positions.empty());

    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();

    for (const auto& position : positions) {
        minZ = std::min(minZ, position.z);
        maxZ = std::max(maxZ, position.z);
    }

    CHECK(minZ == doctest::Approx(-torusRadius).epsilon(EPS));
    CHECK(maxZ == doctest::Approx( torusRadius).epsilon(EPS));
}

TEST_CASE("Torus duplicates seam vertices across stack and sector closures") {
    constexpr float innerRadius = 2.0f;
    constexpr float torusRadius = 0.5f;
    constexpr unsigned int stacks = 6;
    constexpr unsigned int sectors = 8;

    const Torus torus(innerRadius, torusRadius, stacks, sectors);

    const auto positions =
        torus.getVertexData().getAttribute<glm::vec3>(pixelforge::graphics::POSITION);

    REQUIRE(positions.size() == static_cast<std::size_t>((stacks + 1) * (sectors + 1)));

    // Same stack, first and last sector should coincide.
    for (unsigned int stack = 0; stack <= stacks; ++stack) {
        const std::size_t first = stack * (sectors + 1);
        const std::size_t last  = first + sectors;

        CHECK(glm::length(positions[first] - positions[last]) <= EPS);
    }

    // First and last stack rows should coincide pointwise.
    for (unsigned int sector = 0; sector <= sectors; ++sector) {
        const std::size_t first = sector;
        const std::size_t last  = stacks * (sectors + 1) + sector;

        CHECK(glm::length(positions[first] - positions[last]) <= EPS);
    }
}
