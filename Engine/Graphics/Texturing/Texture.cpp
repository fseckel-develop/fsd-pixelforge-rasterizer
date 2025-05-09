#include "Texture.h"
#include "../../Managers/TextureManager.h"
#include <SOIL2/SOIL2.h>
#include <fstream>
#include <iostream>
using namespace std;


Texture::Texture():
    textureID(0),
    textureType(0),
    textureUnit(0) {
}


const filesystem::path textureDirectory = "../Resources/Textures";
Texture::Texture(const string& fileName, const GLenum type, const GLint format) {
    this->filePath = textureDirectory / fileName;
    this->textureID = 0;
    this->textureType = type;
    this->textureUnit = TextureManager::NoUnit();
    glGenTextures(1, &textureID);
    glBindTexture(textureType, textureID);
    int width, height;
    if (unsigned char* data = SOIL_load_image(filePath.c_str(), &width, &height, nullptr, SOIL_LOAD_RGBA)) {
        glTexImage2D(textureType, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(textureType);
        SOIL_free_image_data(data);
    }
    else cerr << "Failed to load texture: " << filePath << endl;
    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glBindTexture(textureType, 0);
}


void Texture::SetWrapping(const GLint wrapS, const GLint wrapT) const {
    glBindTexture(textureType, textureID);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrapT);
    glBindTexture(textureType, 0);
}


void Texture::SetFiltering(const GLint minFilter, const GLint magFilter) const {
    glBindTexture(textureType, textureID);
    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, magFilter);
    glBindTexture(textureType, 0);
}


int Texture::BindTexture() {
    if (textureUnit == TextureManager::NoUnit()) textureUnit = TextureManager::GetFreeUnit();
    glActiveTexture(textureUnit);
    glBindTexture(textureType, textureID);
    return static_cast<int>(textureUnit - GL_TEXTURE0);
}


void Texture::UnbindTexture() {
    glBindTexture(textureType, 0);
    TextureManager::ReleaseUnit(textureUnit);
}


void Texture::DeleteTexture() {
    glDeleteTextures(1, &textureID);
    textureID = 0;
}


const string& Texture::GetFilePath() const {
    return filePath;
}


GLuint Texture::GetID() const {
    return textureID;
}


GLenum Texture::GetType() const {
    return textureType;
}


GLenum Texture::GetUnit() const {
    return textureUnit;
}
