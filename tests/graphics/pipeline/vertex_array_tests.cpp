#include <doctest.h>
#include <graphics/pipeline/index_buffer.hpp>
#include <graphics/pipeline/vertex_array.hpp>
#include <graphics/pipeline/vertex_buffer.hpp>
#include <pixelforge/graphics/pipeline/vertex_data.hpp>
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <vector>

using pixelforge::graphics::IndexBuffer;
using pixelforge::graphics::NORMAL;
using pixelforge::graphics::POSITION;
using pixelforge::graphics::TEXTURE;
using pixelforge::graphics::VertexArray;
using pixelforge::graphics::VertexBuffer;
using pixelforge::graphics::VertexData;


TEST_CASE("VertexArray constructor creates a valid OpenGL vertex array object") {
    const VertexArray vao;

    CHECK(vao.getID() != 0);
    CHECK(vao.getVertexBuffers().empty());
    CHECK(vao.getIndexBuffer() == nullptr);
}

TEST_CASE("VertexArray bindVAO binds the vertex array") {
    const VertexArray vao;

    GLint boundVAO = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVAO);
    CHECK(boundVAO == 0);

    vao.bindVAO();

    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVAO);
    CHECK(boundVAO == static_cast<GLint>(vao.getID()));
}

TEST_CASE("VertexArray unbindVAO clears the current vertex array binding") {
    const VertexArray vao;
    vao.bindVAO();

    GLint boundVAO = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVAO);
    REQUIRE(boundVAO == static_cast<GLint>(vao.getID()));

    VertexArray::unbindVAO();

    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVAO);
    CHECK(boundVAO == 0);
}

TEST_CASE("VertexArray addVertexBuffer stores vertex buffer and configures attributes") {
    VertexData vertexData;
    vertexData.setAttribute(POSITION, std::vector<glm::vec3>{
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f}
    });
    vertexData.setAttribute(TEXTURE, std::vector<glm::vec2>{
        {0.0f, 0.0f},
        {1.0f, 0.0f}
    });
    vertexData.setAttribute(NORMAL, std::vector<glm::vec3>{
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f}
    });

    const VertexBuffer vbo(vertexData);
    VertexArray vao;

    vao.addVertexBuffer(vbo);

    REQUIRE(vao.getVertexBuffers().size() == 1);

    vao.bindVAO();

    GLint enabled = 0;
    glGetVertexAttribiv(POSITION, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    CHECK(enabled == 1);

    glGetVertexAttribiv(TEXTURE, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    CHECK(enabled == 1);

    glGetVertexAttribiv(NORMAL, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    CHECK(enabled == 1);

    VertexArray::unbindVAO();
}

TEST_CASE("VertexArray addVertexBuffer stores multiple vertex buffers") {
    VertexData vertexDataA;
    vertexDataA.setAttribute(POSITION, std::vector<glm::vec3>{
        {0.0f, 0.0f, 0.0f}
    });

    VertexData vertexDataB;
    vertexDataB.setAttribute(POSITION, std::vector<glm::vec3>{
        {1.0f, 1.0f, 1.0f}
    });

    const VertexBuffer vboA(vertexDataA);
    const VertexBuffer vboB(vertexDataB);
    VertexArray vao;

    vao.addVertexBuffer(vboA);
    vao.addVertexBuffer(vboB);

    REQUIRE(vao.getVertexBuffers().size() == 2);
}

TEST_CASE("VertexArray setIndexBuffer stores pointer to provided index buffer") {
    const std::vector<GLuint> indices = {0, 1, 2};

    IndexBuffer ibo(indices);
    VertexArray vao;

    vao.setIndexBuffer(ibo);

    REQUIRE(vao.getIndexBuffer() != nullptr);
    CHECK(vao.getIndexBuffer() == &ibo);
}

TEST_CASE("VertexArray setIndexBuffer binds element array buffer to VAO") {
    VertexArray vao;
    IndexBuffer ibo({0, 1, 2});

    vao.setIndexBuffer(ibo);
    vao.bindVAO();

    GLint boundEBO = 0;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundEBO);
    CHECK(boundEBO != 0);

    VertexArray::unbindVAO();
}

TEST_CASE("VertexArray addVertexBuffer leaves no VAO or VBO bound afterwards") {
    VertexData vertexData;
    vertexData.setAttribute(POSITION, std::vector<glm::vec3>{
        {0.0f, 0.0f, 0.0f}
    });

    const VertexBuffer vbo(vertexData);
    VertexArray vao;

    vao.addVertexBuffer(vbo);

    GLint boundVAO = 0;
    GLint boundVBO = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVAO);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundVBO);

    CHECK(boundVAO == 0);
    CHECK(boundVBO == 0);
}

TEST_CASE("VertexArray operations do not produce OpenGL error") {
    glGetError();

    VertexData vertexData;
    vertexData.setAttribute(POSITION, std::vector<glm::vec3>{
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    });

    const VertexBuffer vbo(vertexData);
    IndexBuffer ibo({0, 1, 2});
    VertexArray vao;

    vao.addVertexBuffer(vbo);
    vao.setIndexBuffer(ibo);

    CHECK(glGetError() == GL_NO_ERROR);
}
