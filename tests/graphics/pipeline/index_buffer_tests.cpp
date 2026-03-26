#include <doctest.h>
#include <graphics/pipeline/index_buffer.hpp>
#include <GL/glew.h>
#include <vector>

using pixelforge::graphics::IndexBuffer;


TEST_CASE("IndexBuffer constructor creates bindable OpenGL element array buffer") {
    const std::vector<GLuint> indices = {0, 1, 2, 2, 3, 0};

    const IndexBuffer ibo(indices);

    GLint boundBuffer = 0;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundBuffer);
    CHECK(boundBuffer == 0);

    ibo.bindIBO();

    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundBuffer);
    CHECK(boundBuffer != 0);
}

TEST_CASE("IndexBuffer handles empty index data without OpenGL errors") {
    glGetError();

    const std::vector<GLuint> indices; // empty
    const IndexBuffer ibo(indices);

    CHECK(glGetError() == GL_NO_ERROR);

    ibo.bindIBO();
    CHECK(glGetError() == GL_NO_ERROR);

    IndexBuffer::unbindIBO();
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("IndexBuffer bindIBO binds a real OpenGL element array buffer") {
    const std::vector<GLuint> indices = {0, 1, 2};

    const IndexBuffer ibo(indices);

    GLint boundBuffer = 0;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundBuffer);
    CHECK(boundBuffer == 0);

    ibo.bindIBO();

    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundBuffer);
    CHECK(boundBuffer != 0);
}

TEST_CASE("IndexBuffer unbindIBO clears current OpenGL element array buffer binding") {
    const std::vector<GLuint> indices = {0, 1, 2};

    const IndexBuffer ibo(indices);
    ibo.bindIBO();

    GLint boundBuffer = 0;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundBuffer);
    REQUIRE(boundBuffer != 0);

    IndexBuffer::unbindIBO();

    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundBuffer);
    CHECK(boundBuffer == 0);
}

TEST_CASE("IndexBuffer upload does not produce OpenGL error for valid index data") {
    glGetError();

    const std::vector<GLuint> indices = {0, 1, 2, 2, 3, 0};
    const IndexBuffer ibo(indices);

    const GLenum error = glGetError();
    CHECK(error == GL_NO_ERROR);

    ibo.bindIBO();
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("IndexBuffer can be manually deleted without immediate OpenGL error") {
    const std::vector<GLuint> indices = {0, 1, 2};

    IndexBuffer ibo(indices);

    glGetError();
    ibo.deleteIBO();
    const GLenum error = glGetError();

    CHECK(error == GL_NO_ERROR);
}
