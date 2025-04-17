#include "VertexBuffer.h"
#include <ostream>
using namespace std;
using namespace glm;


VertexBuffer::VertexBuffer(const VertexData& vertexData):
    vertexBufferID(0) {
    glGenBuffers(1, &vertexBufferID);
    BindVBO();
    glBufferData(GL_ARRAY_BUFFER, vertexData.GetBufferSize(), vertexData.GetBufferData(), GL_STATIC_DRAW);
    CreateBufferLayout(vertexData);
    UnbindVBO();
}


void VertexBuffer::CreateBufferLayout(const VertexData& vertexData) {
    for (const auto& [name, data] : vertexData.GetAttributes()) {
        visit([&]<typename Type>([[maybe_unused]] const Type& attributeData) {
            if constexpr (std::is_same_v<Type, std::vector<vec2>>) {
                bufferLayout.AddElement<vec2>(name);
            } else if constexpr (std::is_same_v<Type, std::vector<vec3>>) {
                bufferLayout.AddElement<vec3>(name);
            }
        }, data);
    }
}


void VertexBuffer::BindVBO() const {
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
}


void VertexBuffer::UnbindVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


VertexBufferLayout VertexBuffer::GetBufferLayout() const {
    return bufferLayout;
}


void VertexBuffer::DeleteVBO() const {
    glDeleteBuffers(1, &vertexBufferID);
}


VertexBuffer::~VertexBuffer() {
    DeleteVBO();
}
