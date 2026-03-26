#include <doctest.h>
#include <pixelforge/graphics/pipeline/vertex_data.hpp>
#include <graphics/pipeline/vertex_buffer.hpp>
#include <GL/glew.h>
#include <GLM/glm.hpp>

using pixelforge::graphics::NORMAL;
using pixelforge::graphics::POSITION;
using pixelforge::graphics::TEXTURE;
using pixelforge::graphics::VertexBuffer;
using pixelforge::graphics::VertexData;


TEST_CASE("VertexBuffer constructor uploads data and creates non-empty layout") {
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

    const VertexBuffer vbo(vertexData);
    const auto layout = vbo.getBufferLayout();

    CHECK_FALSE(layout.getElements().empty());
    CHECK(layout.getElements().size() == 3);
    CHECK(layout.getStride() > 0);
}

TEST_CASE("VertexBuffer bindVBO binds a real OpenGL array buffer") {
    VertexData vertexData;
    vertexData.setAttribute(POSITION, std::vector<glm::vec3>{
        {0.0f, 0.0f, 0.0f}
    });

    const VertexBuffer vbo(vertexData);

    GLint boundBuffer = 0;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundBuffer);
    CHECK(boundBuffer == 0);

    vbo.bindVBO();

    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundBuffer);
    CHECK(boundBuffer != 0);
}

TEST_CASE("VertexBuffer unbindVBO clears current OpenGL array buffer binding") {
    VertexData vertexData;
    vertexData.setAttribute(POSITION, std::vector<glm::vec3>{
        {0.0f, 0.0f, 0.0f}
    });

    const VertexBuffer vbo(vertexData);
    vbo.bindVBO();

    GLint boundBuffer = 0;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundBuffer);
    REQUIRE(boundBuffer != 0);

    VertexBuffer::unbindVBO();

    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundBuffer);
    CHECK(boundBuffer == 0);
}

TEST_CASE("VertexBuffer createBufferLayout reflects attribute types in vertex data") {
    VertexData vertexData;
    vertexData.setAttribute(POSITION, std::vector<glm::vec3>{
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f}
    });
    vertexData.setAttribute(TEXTURE, std::vector<glm::vec2>{
        {0.0f, 0.0f},
        {1.0f, 0.0f}
    });

    const VertexBuffer vbo(vertexData);
    const auto layout = vbo.getBufferLayout();
    const auto& elements = layout.getElements();

    REQUIRE(elements.size() == 2);

    CHECK(elements[0].size == 3);
    CHECK(elements[0].type == GL_FLOAT);
    CHECK(elements[0].normalized == GL_FALSE);
    CHECK(elements[0].index == POSITION);

    CHECK(elements[1].size == 2);
    CHECK(elements[1].type == GL_FLOAT);
    CHECK(elements[1].normalized == GL_FALSE);
    CHECK(elements[1].index == TEXTURE);
}

TEST_CASE("VertexBuffer stride matches combined attribute sizes") {
    VertexData vertexData;
    vertexData.setAttribute(POSITION, std::vector<glm::vec3>{
        {0.0f, 0.0f, 0.0f}
    });
    vertexData.setAttribute(TEXTURE, std::vector<glm::vec2>{
        {0.0f, 0.0f}
    });
    vertexData.setAttribute(NORMAL, std::vector<glm::vec3>{
        {0.0f, 0.0f, 1.0f}
    });

    const VertexBuffer vbo(vertexData);
    const auto layout = vbo.getBufferLayout();

    CHECK(layout.getStride() == sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3));
}

TEST_CASE("VertexBuffer can be manually deleted without OpenGL error") {
    VertexData vertexData;
    vertexData.setAttribute(POSITION, std::vector<glm::vec3>{
        {0.0f, 0.0f, 0.0f}
    });

    VertexBuffer vbo(vertexData);

    glGetError();
    vbo.deleteVBO();
    const GLenum error = glGetError();

    CHECK(error == GL_NO_ERROR);
}
