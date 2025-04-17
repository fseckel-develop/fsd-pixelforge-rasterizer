#pragma once
#include <GL/glew.h>
#include <string>
using namespace std;


class Texture final {
public:
    explicit Texture(const string&, GLenum = GL_TEXTURE_2D, GLint = GL_RGBA);
    explicit Texture(const vector<string>&);
    void SetWrapping(GLint, GLint) const;
    void SetFiltering(GLint, GLint) const;
    int BindTexture();
    void UnbindTexture();
    void DeleteTexture();
    ~Texture();

private:
    GLuint textureID;
    GLenum textureType;
    GLenum textureUnit;
};
