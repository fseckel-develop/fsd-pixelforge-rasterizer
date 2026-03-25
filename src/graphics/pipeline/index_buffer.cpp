#include <vector>
#include "graphics/pipeline/index_buffer.hpp"


namespace pixelforge::graphics {

    IndexBuffer::IndexBuffer(const std::vector<GLuint>& data):
        indexBufferID_(0) {
        glGenBuffers(1, &indexBufferID_);
        bindIBO();
        const auto size = static_cast<GLsizeiptr>(data.size() * sizeof(GLuint));
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, &data[0], GL_STATIC_DRAW);
        unbindIBO();
    }


    void IndexBuffer::bindIBO() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID_);
    }


    void IndexBuffer::unbindIBO() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }


    void IndexBuffer::deleteIBO() const {
        glDeleteBuffers(1, &indexBufferID_);
    }


    IndexBuffer::~IndexBuffer() {
        deleteIBO();
    }

} // namespace pixelforge::graphics
