#include "CubeMap.h"
#include "../../Managers/TextureManager.h"
#include <SOIL2/SOIL2.h>
#include <fstream>
#include <iostream>
using namespace std;


const filesystem::path cubeMapDirectory = "../Resources/Textures/CubeMaps";
CubeMap::CubeMap(const vector<string>& faces) {
    this->filePath = cubeMapDirectory / faces[0];
    this->textureID = 0;
    this->textureType = GL_TEXTURE_CUBE_MAP;
    this->textureUnit = TextureManager::NoUnit();
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
    glTexParameteri(textureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glBindTexture(textureType, 0);
}


void CubeMap::SetWrapping(const GLint wrapR, const GLint wrapS, const GLint wrapT) const {
    glBindTexture(textureType, textureID);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_R, wrapR);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrapT);
    glBindTexture(textureType, 0);
}
