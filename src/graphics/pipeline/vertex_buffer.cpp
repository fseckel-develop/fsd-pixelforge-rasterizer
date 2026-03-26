#include "graphics/pipeline/vertex_buffer.hpp"


namespace pixelforge::graphics {

    using glm::vec2;
    using glm::vec3;


    VertexBuffer::VertexBuffer(const VertexData& vertexData):
        vertexBufferID_(0) {
        glGenBuffers(1, &vertexBufferID_);
        bindVBO();
        glBufferData(GL_ARRAY_BUFFER, vertexData.getBufferSize(), vertexData.getInterleavedBufferPointer(), GL_STATIC_DRAW);
        createBufferLayout(vertexData);
        unbindVBO();
    }


    VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept :
        vertexBufferID_(other.vertexBufferID_),
        bufferLayout_(std::move(other.bufferLayout_)) {
        other.vertexBufferID_ = 0;
    }


    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
        if (this != &other) {
            deleteVBO();
            vertexBufferID_ = other.vertexBufferID_;
            bufferLayout_ = std::move(other.bufferLayout_);
            other.vertexBufferID_ = 0;
        }
        return *this;
    }


    void VertexBuffer::createBufferLayout(const VertexData& vertexData) {
        for (const auto& [attribute, data] : vertexData.getAttributes()) {
            visit([&]<typename Type>([[maybe_unused]] const Type& attributeData) {
                if constexpr (std::is_same_v<Type, std::vector<vec2>>) {
                    bufferLayout_.addElement<vec2>(attribute);
                } else if constexpr (std::is_same_v<Type, std::vector<vec3>>) {
                    bufferLayout_.addElement<vec3>(attribute);
                }
            }, data);
        }
    }


    void VertexBuffer::bindVBO() const {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID_);
    }


    void VertexBuffer::unbindVBO() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    VertexBufferLayout VertexBuffer::getBufferLayout() const {
        return bufferLayout_;
    }


    void VertexBuffer::deleteVBO() {
        if (vertexBufferID_ != 0) {
            glDeleteBuffers(1, &vertexBufferID_);
            vertexBufferID_ = 0;
        }
    }


    VertexBuffer::~VertexBuffer() {
        deleteVBO();
    }

} // namespace pixelforge::graphics
