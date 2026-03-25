#include "graphics/pipeline/vertex_array.hpp"
#include "graphics/pipeline/vertex_buffer.hpp"
#include "graphics/pipeline/index_buffer.hpp"


namespace pixelforge::graphics {

    using std::vector;


    VertexArray::VertexArray():
        vertexArrayID_(0) {
        glGenVertexArrays(1, &vertexArrayID_);
        indexBuffer_ = nullptr;
    }


    void VertexArray::bindVAO() const {
        glBindVertexArray(vertexArrayID_);
    }


    void VertexArray::unbindVAO() {
        glBindVertexArray(0);
    }


    void VertexArray::addVertexBuffer(const VertexBuffer& vertexBuffer) {
        bindVAO();
        vertexBuffer.bindVBO();
        const auto& layout = vertexBuffer.getBufferLayout(); // NOLINT
        for (const auto& element : layout.getElements()) { // NOLINT
            glVertexAttribPointer(
                element.index,
                element.size,
                element.type,
                element.normalized,
                layout.getStride(),
                reinterpret_cast<const void *>(element.offset)
            );
            glEnableVertexAttribArray(element.index);
        }
        vertexBuffers_.push_back(vertexBuffer);
        unbindVAO();
        VertexBuffer::unbindVBO();
    }


    void VertexArray::setIndexBuffer(const IndexBuffer& indexBuffer) {
        bindVAO();
        indexBuffer.bindIBO();
        indexBuffer_ = &indexBuffer;
        unbindVAO();
    }


    const vector<VertexBuffer>& VertexArray::getVertexBuffers() const {
        return vertexBuffers_;
    }


    const IndexBuffer* VertexArray::getIndexBuffer() const {
        return indexBuffer_;
    }


    GLuint VertexArray::getID() const {
        return vertexArrayID_;
    }


    void VertexArray::deleteVAO() const {
        indexBuffer_->deleteIBO();
        for (const auto& vertexBuffer : vertexBuffers_) {
            vertexBuffer.deleteVBO();
        }
        glDeleteVertexArrays(1, &vertexArrayID_);
    }


    VertexArray::~VertexArray() {
        deleteVAO();
    }

} // namespace pixelforge::graphics
