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
    CubeMap::CubeMap(const std::string& baseName) {
        const path baseDirectory = cubeMapDirectory / baseName;
        const std::vector<std::pair<std::string, GLenum>> faces = {
            { baseName + "_right",  GL_TEXTURE_CUBE_MAP_POSITIVE_X },
            { baseName + "_left",   GL_TEXTURE_CUBE_MAP_NEGATIVE_X },
            { baseName + "_top",    GL_TEXTURE_CUBE_MAP_POSITIVE_Y },
            { baseName + "_bottom", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y },
            { baseName + "_front",  GL_TEXTURE_CUBE_MAP_POSITIVE_Z },
            { baseName + "_back",   GL_TEXTURE_CUBE_MAP_NEGATIVE_Z }
        };
        filePath_ = baseDirectory.string();
        textureID_ = 0;
        textureType_ = GL_TEXTURE_CUBE_MAP;
        textureUnit_ = management::TextureManager::noUnit();
        glGenTextures(1, &textureID_);
        glBindTexture(textureType_, textureID_);
        int width, height;
        bool hadMissingFace = false;
        const std::vector<std::string> extensions = {".jpg", ".png", ".tga"};
        for (const auto& [fileName, target] : faces) {
            unsigned char* data = nullptr;
            for (const auto& ext : extensions) {
                path fullPath = baseDirectory / (fileName + ext);
                data = SOIL_load_image(fullPath.c_str(), &width, &height, nullptr, SOIL_LOAD_RGBA);
                if (data) break;
            }
            if (data) {
                glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                SOIL_free_image_data(data);
            } else {
                hadMissingFace = true;
                constexpr unsigned char fallback[] = {255, 0, 255, 255}; // magenta
                glTexImage2D(target, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, fallback);
            }
        }
#ifndef NDEBUG
        if (hadMissingFace) {
            cerr << "CubeMap '" << baseName << "' used fallback for one or more faces." << endl;
        }
#endif
        glTexParameteri(textureType_, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(textureType_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(textureType_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(textureType_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(textureType_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
