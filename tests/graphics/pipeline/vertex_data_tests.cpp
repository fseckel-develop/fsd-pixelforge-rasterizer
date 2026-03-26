#include <doctest.h>
#include <pixelforge/graphics/pipeline/vertex_data.hpp>
#include <GLM/glm.hpp>
#include <cstring>
#include <vector>

using pixelforge::graphics::COLOR;
using pixelforge::graphics::NORMAL;
using pixelforge::graphics::POSITION;
using pixelforge::graphics::TEXTURE;
using pixelforge::graphics::VertexData;


namespace {

    constexpr float EPS = 1e-6f;

    template <typename T>
    T readValueAt(const std::vector<uint8_t>& buffer, const std::size_t offset) {
        T value{};
        std::memcpy(&value, buffer.data() + offset, sizeof(T));
        return value;
    }

    void checkVec2Approx(const glm::vec2& actual, const glm::vec2& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
    }

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
    }

} // namespace


TEST_CASE("VertexData starts empty") {
    const VertexData vertexData;

    CHECK(vertexData.getVertexCount() == 0);
    CHECK(vertexData.getAttributes().empty());
    CHECK(vertexData.getInterleavedBuffer().empty());
    CHECK(vertexData.getInterleavedBufferPointer() == nullptr);
    CHECK(vertexData.getBufferSize() == 0);
    CHECK_FALSE(vertexData.hasAttribute(POSITION));
    CHECK_FALSE(vertexData.hasAttribute(TEXTURE));
    CHECK_FALSE(vertexData.hasAttribute(NORMAL));
    CHECK_FALSE(vertexData.hasAttribute(COLOR));
}

TEST_CASE("VertexData setAttribute POSITION sets vertex count and stores attribute") {
    VertexData vertexData;

    const std::vector<glm::vec3> positions = {
        {0.0f, 1.0f, 2.0f},
        {3.0f, 4.0f, 5.0f}
    };

    vertexData.setAttribute(POSITION, positions);

    CHECK(vertexData.hasAttribute(POSITION));
    CHECK(vertexData.getVertexCount() == 2);

    const auto& stored = vertexData.getAttribute<glm::vec3>(POSITION);
    REQUIRE(stored.size() == 2);
    checkVec3Approx(stored[0], positions[0]);
    checkVec3Approx(stored[1], positions[1]);
}

TEST_CASE("VertexData ignores non-position attributes before POSITION is set") {
    VertexData vertexData;

    vertexData.setAttribute(TEXTURE, std::vector<glm::vec2>{
        {0.0f, 0.0f}
    });

    CHECK_FALSE(vertexData.hasAttribute(TEXTURE));
    CHECK(vertexData.getVertexCount() == 0);
}

TEST_CASE("VertexData getAttribute returns empty vector for missing attribute") {
    const VertexData vertexData;

    CHECK(vertexData.getAttribute<glm::vec3>(POSITION).empty());
    CHECK(vertexData.getAttribute<glm::vec2>(TEXTURE).empty());
}

TEST_CASE("VertexData rejects non-position attribute with mismatched vertex count") {
    VertexData vertexData;

    vertexData.setAttribute(POSITION, std::vector<glm::vec3>{
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f}
    });

    vertexData.setAttribute(TEXTURE, std::vector<glm::vec2>{
        {0.0f, 0.0f}
    });

    CHECK_FALSE(vertexData.hasAttribute(TEXTURE));
    CHECK(vertexData.getVertexCount() == 2);
}

TEST_CASE("VertexData stores multiple attributes with matching vertex count") {
    VertexData vertexData;

    const std::vector<glm::vec3> positions = {
        {0.0f, 1.0f, 2.0f},
        {3.0f, 4.0f, 5.0f}
    };
    const std::vector<glm::vec2> texCoords = {
        {0.0f, 0.5f},
        {1.0f, 0.25f}
    };
    const std::vector<glm::vec3> normals = {
        {0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f}
    };

    vertexData.setAttribute(POSITION, positions);
    vertexData.setAttribute(TEXTURE, texCoords);
    vertexData.setAttribute(NORMAL, normals);

    CHECK(vertexData.hasAttribute(POSITION));
    CHECK(vertexData.hasAttribute(TEXTURE));
    CHECK(vertexData.hasAttribute(NORMAL));
    CHECK(vertexData.getVertexCount() == 2);

    REQUIRE(vertexData.getAttribute<glm::vec3>(POSITION).size() == 2);
    REQUIRE(vertexData.getAttribute<glm::vec2>(TEXTURE).size() == 2);
    REQUIRE(vertexData.getAttribute<glm::vec3>(NORMAL).size() == 2);
}

TEST_CASE("VertexData removeAttribute removes stored attribute and updates interleaved buffer") {
    VertexData vertexData;

    vertexData.setAttribute(POSITION, std::vector<glm::vec3>{
        {0.0f, 1.0f, 2.0f},
        {3.0f, 4.0f, 5.0f}
    });
    vertexData.setAttribute(TEXTURE, std::vector<glm::vec2>{
        {0.0f, 0.5f},
        {1.0f, 0.25f}
    });

    const auto sizeWithTexture = vertexData.getBufferSize();
    REQUIRE(sizeWithTexture > 0);

    vertexData.removeAttribute(TEXTURE);

    CHECK_FALSE(vertexData.hasAttribute(TEXTURE));
    CHECK(vertexData.hasAttribute(POSITION));
    CHECK(vertexData.getBufferSize() < sizeWithTexture);
}

TEST_CASE("VertexData clear resets all data") {
    VertexData vertexData;

    vertexData.setAttribute(POSITION, std::vector<glm::vec3>{
        {0.0f, 1.0f, 2.0f},
        {3.0f, 4.0f, 5.0f}
    });
    vertexData.setAttribute(TEXTURE, std::vector<glm::vec2>{
        {0.0f, 0.5f},
        {1.0f, 0.25f}
    });

    vertexData.clear();

    CHECK(vertexData.getVertexCount() == 0);
    CHECK(vertexData.getAttributes().empty());
    CHECK(vertexData.getInterleavedBuffer().empty());
    CHECK(vertexData.getInterleavedBufferPointer() == nullptr);
    CHECK(vertexData.getBufferSize() == 0);
    CHECK_FALSE(vertexData.hasAttribute(POSITION));
    CHECK_FALSE(vertexData.hasAttribute(TEXTURE));
}

TEST_CASE("VertexData interleaves POSITION and TEXTURE in attribute order") {
    VertexData vertexData;

    const std::vector<glm::vec3> positions = {
        {1.0f, 2.0f, 3.0f},
        {4.0f, 5.0f, 6.0f}
    };
    const std::vector<glm::vec2> texCoords = {
        {0.25f, 0.75f},
        {0.5f, 1.0f}
    };

    vertexData.setAttribute(POSITION, positions);
    vertexData.setAttribute(TEXTURE, texCoords);

    const auto& buffer = vertexData.getInterleavedBuffer();
    REQUIRE_FALSE(buffer.empty());

    const std::size_t stride = sizeof(glm::vec3) + sizeof(glm::vec2);
    REQUIRE(buffer.size() == 2 * stride);

    const auto p0 = readValueAt<glm::vec3>(buffer, 0);
    const auto uv0 = readValueAt<glm::vec2>(buffer, sizeof(glm::vec3));
    const auto p1 = readValueAt<glm::vec3>(buffer, stride);
    const auto uv1 = readValueAt<glm::vec2>(buffer, stride + sizeof(glm::vec3));

    checkVec3Approx(p0, positions[0]);
    checkVec2Approx(uv0, texCoords[0]);
    checkVec3Approx(p1, positions[1]);
    checkVec2Approx(uv1, texCoords[1]);
}

TEST_CASE("VertexData interleaves attributes sorted by VertexAttribute key") {
    VertexData vertexData;

    const std::vector<glm::vec3> positions = {
        {1.0f, 2.0f, 3.0f}
    };
    const std::vector<glm::vec2> texCoords = {
        {0.25f, 0.75f}
    };
    const std::vector<glm::vec3> normals = {
        {0.0f, 0.0f, 1.0f}
    };
    const std::vector<glm::vec3> colors = {
        {0.1f, 0.2f, 0.3f}
    };

    vertexData.setAttribute(POSITION, positions);
    vertexData.setAttribute(COLOR, colors);
    vertexData.setAttribute(TEXTURE, texCoords);
    vertexData.setAttribute(NORMAL, normals);

    const auto& buffer = vertexData.getInterleavedBuffer();
    REQUIRE_FALSE(buffer.empty());

    std::size_t offset = 0;
    const auto p = readValueAt<glm::vec3>(buffer, offset);
    offset += sizeof(glm::vec3);
    const auto uv = readValueAt<glm::vec2>(buffer, offset);
    offset += sizeof(glm::vec2);
    const auto n = readValueAt<glm::vec3>(buffer, offset);
    offset += sizeof(glm::vec3);
    const auto c = readValueAt<glm::vec3>(buffer, offset);

    checkVec3Approx(p, positions[0]);
    checkVec2Approx(uv, texCoords[0]);
    checkVec3Approx(n, normals[0]);
    checkVec3Approx(c, colors[0]);
}

TEST_CASE("VertexData buffer size matches vertex count times stride") {
    VertexData vertexData;

    vertexData.setAttribute(POSITION, std::vector<glm::vec3>{
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    });
    vertexData.setAttribute(TEXTURE, std::vector<glm::vec2>{
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {0.0f, 1.0f}
    });
    vertexData.setAttribute(NORMAL, std::vector<glm::vec3>{
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f}
    });

    const std::size_t stride = sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3);
    CHECK(vertexData.getBufferSize() == static_cast<GLsizeiptr>(3 * stride));
}

TEST_CASE("VertexData interleaved buffer pointer matches buffer storage when non-empty") {
    VertexData vertexData;

    vertexData.setAttribute(POSITION, std::vector<glm::vec3>{
        {0.0f, 0.0f, 0.0f}
    });

    REQUIRE_FALSE(vertexData.getInterleavedBuffer().empty());
    CHECK(vertexData.getInterleavedBufferPointer() == vertexData.getInterleavedBuffer().data());
}
