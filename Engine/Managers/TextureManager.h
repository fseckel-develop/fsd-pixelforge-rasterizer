#pragma once
#include "InstanceManager.h"
#include "../Graphics/Texturing/Texture.h"


class TextureManager final : public InstanceManager<Texture, TextureManager> {
public:
    static size_t Hash(const Texture& texture) {
        size_t seed = 0;
        CombineHashes(seed, hash<string>{}(texture.GetFilePath()));
        CombineHashes(seed, hash<GLenum>{}(texture.GetType()));
        return seed;
    }

    static bool Equals(const Texture& left, const Texture& right) {
        return left.GetFilePath() == right.GetFilePath() &&
            left.GetType() == right.GetType();
    }

    static GLenum GetFreeUnit() {
        for (auto& unit : textureUnits) { // NOLINT
            if (!unit.isOccupied) {
                unit.isOccupied = true;
                return unit.spot;
            }
        }
        throw runtime_error("No available texture units");
    }

    static void ReleaseUnit(GLenum& spot) {
        for (auto& textureUnit : textureUnits) { // NOLINT
            if (textureUnit.spot == spot) {
                textureUnit.isOccupied = false;
                spot = noUnit;
                return;
            }
        }
    }

    static GLenum NoUnit() {
        return noUnit;
    }

    static void Initialize() {
        GLint maxUnits;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnits);
        textureUnits.resize(maxUnits, {GL_TEXTURE0, false});
        for (GLint i = 0; i < maxUnits; i++) {
            textureUnits[i].spot = GL_TEXTURE0 + i;
        }
    }

private:
    struct TextureUnit { GLenum spot; bool isOccupied; };
    inline static vector<TextureUnit> textureUnits;
    static constexpr GLenum noUnit = static_cast<GLenum>(-1);
};
