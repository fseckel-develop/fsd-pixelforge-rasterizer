#include <doctest.h>
#include <graphics/pipeline/vertex_buffer_layout.hpp>
#include <GLM/glm.hpp>

using pixelforge::graphics::COLOR;
using pixelforge::graphics::NORMAL;
using pixelforge::graphics::POSITION;
using pixelforge::graphics::TEXTURE;
using pixelforge::graphics::VertexBufferElement;
using pixelforge::graphics::VertexBufferLayout;


TEST_CASE("VertexBufferLayout starts empty with zero stride") {
    const VertexBufferLayout layout;

    CHECK(layout.getElements().empty());
    CHECK(layout.getStride() == 0);
}

TEST_CASE("VertexBufferLayout addElement glm vec2 adds correct element") {
    VertexBufferLayout layout;
    layout.addElement<glm::vec2>(TEXTURE);

    const auto& elements = layout.getElements();
    REQUIRE(elements.size() == 1);

    CHECK(elements[0].index == TEXTURE);
    CHECK(elements[0].size == 2);
    CHECK(elements[0].type == GL_FLOAT);
    CHECK(elements[0].normalized == GL_FALSE);
    CHECK(elements[0].offset == 0);

    CHECK(layout.getStride() == static_cast<GLsizei>(sizeof(glm::vec2)));
}

TEST_CASE("VertexBufferLayout addElement glm vec3 adds correct element") {
    VertexBufferLayout layout;
    layout.addElement<glm::vec3>(POSITION);

    const auto& elements = layout.getElements();
    REQUIRE(elements.size() == 1);

    CHECK(elements[0].index == POSITION);
    CHECK(elements[0].size == 3);
    CHECK(elements[0].type == GL_FLOAT);
    CHECK(elements[0].normalized == GL_FALSE);
    CHECK(elements[0].offset == 0);

    CHECK(layout.getStride() == static_cast<GLsizei>(sizeof(glm::vec3)));
}

TEST_CASE("VertexBufferLayout adds multiple elements with correct offsets and stride") {
    VertexBufferLayout layout;
    layout.addElement<glm::vec3>(POSITION);
    layout.addElement<glm::vec2>(TEXTURE);
    layout.addElement<glm::vec3>(NORMAL);

    const auto& elements = layout.getElements();
    REQUIRE(elements.size() == 3);

    CHECK(elements[0].index == POSITION);
    CHECK(elements[0].offset == 0);

    CHECK(elements[1].index == TEXTURE);
    CHECK(elements[1].offset == static_cast<GLsizei>(sizeof(glm::vec3)));

    CHECK(elements[2].index == NORMAL);
    CHECK(elements[2].offset == static_cast<GLsizei>(sizeof(glm::vec3) + sizeof(glm::vec2)));

    CHECK(layout.getStride() ==
          static_cast<GLsizei>(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3)));
}

TEST_CASE("VertexBufferLayout removeElement removes matching element and recomputes layout") {
    VertexBufferLayout layout;
    layout.addElement<glm::vec3>(POSITION);
    layout.addElement<glm::vec2>(TEXTURE);
    layout.addElement<glm::vec3>(NORMAL);

    layout.removeElement(TEXTURE);

    const auto& elements = layout.getElements();
    REQUIRE(elements.size() == 2);

    CHECK(elements[0].index == POSITION);
    CHECK(elements[0].offset == 0);

    CHECK(elements[1].index == NORMAL);
    CHECK(elements[1].offset == static_cast<GLsizei>(sizeof(glm::vec3)));

    CHECK(layout.getStride() ==
          static_cast<GLsizei>(sizeof(glm::vec3) + sizeof(glm::vec3)));
}

TEST_CASE("VertexBufferLayout removeElement ignores missing element") {
    VertexBufferLayout layout;
    layout.addElement<glm::vec3>(POSITION);
    layout.addElement<glm::vec2>(TEXTURE);

    const auto strideBefore = layout.getStride();
    layout.removeElement(COLOR);

    REQUIRE(layout.getElements().size() == 2);
    CHECK(layout.getStride() == strideBefore);
}

TEST_CASE("VertexBufferElement getTypeSize returns expected byte sizes") {
    CHECK(VertexBufferElement::getTypeSize(GL_UNSIGNED_INT) == 4);
    CHECK(VertexBufferElement::getTypeSize(GL_UNSIGNED_BYTE) == 1);
    CHECK(VertexBufferElement::getTypeSize(GL_FLOAT) == 4);
    CHECK(VertexBufferElement::getTypeSize(GL_INT) == 0);
}
