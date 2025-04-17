#pragma once
#include "VertexData.h"
#include <vector>
#include <GL/glew.h>
#include <GLM/glm.hpp>
using namespace std; using namespace glm;


typedef struct VertexBufferElement {
    VertexAttribute index;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei offset;
    static GLsizei GetTypeSize(const GLenum type) {
        switch (type) {
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
            case GL_FLOAT: return 4;
            default: return 0;
        }
    }
} VertexBufferElement;


class VertexBufferLayout {
public:
    VertexBufferLayout();
    template <typename T> void AddElement(VertexAttribute);
    void RemoveElement(VertexAttribute);
    [[nodiscard]] const vector<VertexBufferElement>& GetElements() const;
    [[nodiscard]] GLsizei GetStride() const;

private:
    std::vector<VertexBufferElement> elements;
    GLsizei stride;
    GLuint nextAvailableIndex;
    void UpdateLayout();
};


template <> inline void VertexBufferLayout::AddElement<vec2>(const VertexAttribute index) {
    const VertexBufferElement newElement = {index, 2, GL_FLOAT, GL_FALSE, stride};
    elements.push_back(newElement);
    stride += VertexBufferElement::GetTypeSize(GL_FLOAT) * 2;
}


template <> inline void VertexBufferLayout::AddElement<vec3>(const VertexAttribute index) {
    const VertexBufferElement newElement = {index, 3, GL_FLOAT, GL_FALSE, stride};
    elements.push_back(newElement);
    stride += VertexBufferElement::GetTypeSize(GL_FLOAT) * 3;
}
