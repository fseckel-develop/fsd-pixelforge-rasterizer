#pragma once
#include <GL/glew.h>
#include <vector>
class VertexBuffer;
class IndexBuffer;
using namespace std;


class VertexArray {
public:
    VertexArray();
    void BindVAO() const;
    static void UnbindVAO();
    void AddVertexBuffer(const VertexBuffer&);
    void SetIndexBuffer(const IndexBuffer&);
    [[nodiscard]] GLuint GetID() const;
    [[nodiscard]] const vector<VertexBuffer>& GetVertexBuffers() const;
    [[nodiscard]] const IndexBuffer* GetIndexBuffer() const;
    void DeleteVAO() const;
    ~VertexArray();

private:
    GLuint vertexArrayID;
    vector<VertexBuffer> vertexBuffers;
    const IndexBuffer* indexBuffer;
};
