#pragma once
#include <GL/glew.h>
#include <vector>
using namespace std;


class IndexBuffer {
public:
    explicit IndexBuffer(const vector<GLuint>&);
    void BindIBO() const;
    static void UnbindIBO();
    void DeleteIBO() const;
    ~IndexBuffer();

private:
    GLuint indexBufferID;
};
