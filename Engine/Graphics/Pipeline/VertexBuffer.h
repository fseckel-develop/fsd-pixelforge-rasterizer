#pragma once
#include <GL/glew.h>
#include "VertexBufferLayout.h"
class VertexData;


class VertexBuffer {
public:
    explicit VertexBuffer(const VertexData&);
    void CreateBufferLayout(const VertexData&);
    void BindVBO() const;
    static void UnbindVBO();
    [[nodiscard]] VertexBufferLayout GetBufferLayout() const;
    void DeleteVBO() const;
    ~VertexBuffer();

private:
    GLuint vertexBufferID;
    VertexBufferLayout bufferLayout;
};
