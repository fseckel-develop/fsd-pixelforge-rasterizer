#include <pixelforge/graphics/texturing/texture.hpp>
#include <SOIL2/SOIL2.h>
#include <fstream>
#include <iostream>
#include "managers/texture_manager.hpp"


namespace pixelforge::graphics {

    using std::filesystem::path;
    using std::string;
    using std::cerr;
    using std::endl;


    Texture::Texture():
        textureID_(0),
        textureType_(0),
        textureUnit_(0) {
    }


    const path textureDirectory = path(PIXELFORGE_ASSET_DIR) / "textures";
    Texture::Texture(const string& fileName, const GLenum type, const GLint format) {
        this->filePath_ = textureDirectory / fileName;
        this->textureID_ = 0;
        this->textureType_ = type;
        this->textureUnit_ = management::TextureManager::noUnit();
        glGenTextures(1, &textureID_);
        glBindTexture(textureType_, textureID_);
        int width, height;
        if (unsigned char* data = SOIL_load_image(filePath_.c_str(), &width, &height, nullptr, SOIL_LOAD_RGBA)) {
            glTexImage2D(textureType_, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(textureType_);
            SOIL_free_image_data(data);
        } else {
#ifndef NDEBUG
            cerr << "Failed to load texture: " << filePath_ << ". Using fallback" << endl;
#endif
            constexpr unsigned char fallback[] = {255, 0, 255, 255}; // magenta
            glTexImage2D(textureType_, 0, format, 1, 1, 0, format, GL_UNSIGNED_BYTE, fallback);
        }
        glTexParameteri(textureType_, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(textureType_, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(textureType_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(textureType_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(textureType_, 0);
    }


    void Texture::setWrapping(const GLint wrapS, const GLint wrapT) const {
        glBindTexture(textureType_, textureID_);
        glTexParameteri(textureType_, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(textureType_, GL_TEXTURE_WRAP_T, wrapT);
        glBindTexture(textureType_, 0);
    }


    void Texture::setFiltering(const GLint minFilter, const GLint magFilter) const {
        glBindTexture(textureType_, textureID_);
        glTexParameteri(textureType_, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(textureType_, GL_TEXTURE_MAG_FILTER, magFilter);
        glBindTexture(textureType_, 0);
    }


    int Texture::bindTexture() {
        if (textureUnit_ == management::TextureManager::noUnit()) textureUnit_ = management::TextureManager::getFreeUnit();
        glActiveTexture(textureUnit_);
        glBindTexture(textureType_, textureID_);
        return static_cast<int>(textureUnit_ - GL_TEXTURE0);
    }


    void Texture::unbindTexture() {
        glBindTexture(textureType_, 0);
        management::TextureManager::releaseUnit(textureUnit_);
    }


    void Texture::deleteTexture() {
        glDeleteTextures(1, &textureID_);
        textureID_ = 0;
    }


    const string& Texture::getFilePath() const {
        return filePath_;
    }


    GLuint Texture::getID() const {
        return textureID_;
    }


    GLenum Texture::getType() const {
        return textureType_;
    }


    GLenum Texture::getUnit() const {
        return textureUnit_;
    }

} // namespace pixelforge::graphics
