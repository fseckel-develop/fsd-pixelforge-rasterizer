#include <vector>
#include "graphics/pipeline/index_buffer.hpp"


namespace pixelforge::graphics {

    IndexBuffer::IndexBuffer(const std::vector<GLuint>& data):
        indexBufferID_(0) {
        glGenBuffers(1, &indexBufferID_);
        bindIBO();
        const auto size = static_cast<GLsizeiptr>(data.size() * sizeof(GLuint));
        const void* bufferData = data.empty() ? nullptr : data.data();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, bufferData, GL_STATIC_DRAW);
        unbindIBO();
    }


    IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept :
        indexBufferID_(other.indexBufferID_) {
        other.indexBufferID_ = 0;
    }


    IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
        if (this != &other) {
            deleteIBO();
            indexBufferID_ = other.indexBufferID_;
            other.indexBufferID_ = 0;
        }
        return *this;
    }


    void IndexBuffer::bindIBO() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID_);
    }


    void IndexBuffer::unbindIBO() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }


    void IndexBuffer::deleteIBO() {
        if (indexBufferID_ != 0) {
            glDeleteBuffers(1, &indexBufferID_);
            indexBufferID_ = 0;
        }
    }


    IndexBuffer::~IndexBuffer() {
        deleteIBO();
    }

} // namespace pixelforge::graphics
