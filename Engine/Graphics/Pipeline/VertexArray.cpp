#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
using namespace std;


VertexArray::VertexArray():
    vertexArrayID(0) {
    glGenVertexArrays(1, &vertexArrayID);
    indexBuffer = nullptr;
}


void VertexArray::BindVAO() const {
    glBindVertexArray(vertexArrayID);
}


void VertexArray::UnbindVAO() {
    glBindVertexArray(0);
}


void VertexArray::AddVertexBuffer(const VertexBuffer& vertexBuffer) {
    BindVAO();
    vertexBuffer.BindVBO();
    const auto& layout = vertexBuffer.GetBufferLayout(); // NOLINT
    for (const auto& element : layout.GetElements()) { // NOLINT
        glVertexAttribPointer(
            element.index,
            element.size,
            element.type,
            element.normalized,
            layout.GetStride(),
            reinterpret_cast<const void *>(element.offset)
        );
        glEnableVertexAttribArray(element.index);
    }
    vertexBuffers.push_back(vertexBuffer);
    UnbindVAO();
    VertexBuffer::UnbindVBO();
}


void VertexArray::SetIndexBuffer(const IndexBuffer& buffer) {
    BindVAO();
    buffer.BindIBO();
    indexBuffer = &buffer;
    UnbindVAO();
}


const vector<VertexBuffer>& VertexArray::GetVertexBuffers() const {
    return vertexBuffers;
}


const IndexBuffer* VertexArray::GetIndexBuffer() const {
    return indexBuffer;
}


GLuint VertexArray::GetID() const {
    return vertexArrayID;
}


void VertexArray::DeleteVAO() const {
    indexBuffer->DeleteIBO();
    for (const auto& vertexBuffer : vertexBuffers) {
        vertexBuffer.DeleteVBO();
    }
    glDeleteVertexArrays(1, &vertexArrayID);
}


VertexArray::~VertexArray() {
    DeleteVAO();
}
