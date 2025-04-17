#include "IndexBuffer.h"
#include <iostream>


IndexBuffer::IndexBuffer(const vector<GLuint>& data):
    indexBufferID(0) {
    glGenBuffers(1, &indexBufferID);
    BindIBO();
    const auto size = static_cast<GLsizeiptr>(data.size() * sizeof(GLuint));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, &data[0], GL_STATIC_DRAW);
    UnbindIBO();
}


void IndexBuffer::BindIBO() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
}


void IndexBuffer::UnbindIBO() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void IndexBuffer::DeleteIBO() const {
    glDeleteBuffers(1, &indexBufferID);
}


IndexBuffer::~IndexBuffer() {
    DeleteIBO();
}
