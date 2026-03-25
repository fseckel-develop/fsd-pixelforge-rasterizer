#include <pixelforge/graphics/texturing/cube_map.hpp>
#include <SOIL2/SOIL2.h>
#include <fstream>
#include <iostream>
#include "managers/texture_manager.hpp"


namespace pixelforge::graphics {

    using std::filesystem::path;
    using std::vector;
    using std::string;
    using std::cerr;
    using std::endl;


    const path cubeMapDirectory = path(PIXELFORGE_ASSET_DIR) / "textures" / "cube_maps";
    CubeMap::CubeMap(const vector<string>& faces) {
        this->filePath_ = cubeMapDirectory / faces[0];
        this->textureID_ = 0;
        this->textureType_ = GL_TEXTURE_CUBE_MAP;
        this->textureUnit_ = management::TextureManager::noUnit();
        glGenTextures(1, &textureID_);
        glBindTexture(textureType_, textureID_);
        int width, height;
        for (size_t i = 0; i < faces.size(); i++) {
            if (unsigned char* data = SOIL_load_image(faces[i].c_str(), &width, &height, nullptr, SOIL_LOAD_RGBA)) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                SOIL_free_image_data(data);
            }
            else cerr << "Failed to load cube-map face: " << faces[i] << endl;
        }
        glTexParameteri(textureType_, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(textureType_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(textureType_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(textureType_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(textureType_, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glBindTexture(textureType_, 0);
    }


    void CubeMap::setWrapping(const GLint wrapR, const GLint wrapS, const GLint wrapT) const {
        glBindTexture(textureType_, textureID_);
        glTexParameteri(textureType_, GL_TEXTURE_WRAP_R, wrapR);
        glTexParameteri(textureType_, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(textureType_, GL_TEXTURE_WRAP_T, wrapT);
        glBindTexture(textureType_, 0);
    }

} // namespace pixelforge::graphics
