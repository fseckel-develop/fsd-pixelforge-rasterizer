#include "Texture.h"
#include "TextureUnitManager.h"
#include <SOIL2/SOIL2.h>
#include <iostream>


Texture::Texture(const string& filePath, const GLenum type, const GLint format) {
    this->textureID = 0;
    this->textureType = type;
    this->textureUnit = TextureUnitManager::NoUnit();
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


// TODO: Moving to an own CubeMap class
Texture::Texture(const vector<string>& faces) {
    this->textureID = 0;
    this->textureType = GL_TEXTURE_CUBE_MAP;
    this->textureUnit = TextureUnitManager::NoUnit();
    glGenTextures(1, &textureID);
    glBindTexture(textureType, textureID);
    int width, height;
    for (size_t i = 0; i < faces.size(); i++) {
        if (unsigned char* data = SOIL_load_image(faces[i].c_str(), &width, &height, nullptr, SOIL_LOAD_RGBA)) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            SOIL_free_image_data(data);
        }
        else cerr << "Failed to load cube-map face: " << faces[i] << endl;
    }
    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
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
    if (textureUnit == TextureUnitManager::NoUnit()) textureUnit = TextureUnitManager::GetFreeUnit();
    glActiveTexture(textureUnit);
    glBindTexture(textureType, textureID);
    return static_cast<int>(textureUnit - GL_TEXTURE0);
}


void Texture::UnbindTexture() {
    glBindTexture(textureType, 0);
    TextureUnitManager::ReleaseUnit(textureUnit);
    textureUnit = TextureUnitManager::NoUnit();
}


void Texture::DeleteTexture() {
    glDeleteTextures(1, &textureID);
    textureID = 0;
}


Texture::~Texture() {
    DeleteTexture();
}
