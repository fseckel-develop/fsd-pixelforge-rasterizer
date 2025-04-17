#include "TextureUnitManager.h"


vector<TextureUnitManager::TextureUnit> TextureUnitManager::textureUnits;


void TextureUnitManager::Initialize() {
    GLint maxUnits;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnits);
    textureUnits.resize(maxUnits, {GL_TEXTURE0, false});
    for (GLint i = 0; i < maxUnits; i++) {
        textureUnits[i].spot = GL_TEXTURE0 + i;
    }
}


GLenum TextureUnitManager::GetFreeUnit() {
    for (auto& textureUnit : textureUnits) {
        if (!textureUnit.isOccupied) {
            textureUnit.isOccupied = true;
            return textureUnit.spot;
        }
    }
    throw runtime_error("No available texture units");
}


void TextureUnitManager::ReleaseUnit(const GLenum spot) {
    for (auto& textureUnit : textureUnits) {
        if (textureUnit.spot == spot) {
            textureUnit.isOccupied = false;
            return;
        }
    }
}


GLenum TextureUnitManager::NoUnit() {
    return noUnit;
}
