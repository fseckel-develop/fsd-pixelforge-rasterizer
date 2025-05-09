#pragma once
#include "InstanceManager.h"
#include "../Graphics/Texturing/Texture.h"


/// Extension of the InstanceManager class for managing texture instances, ensuring
/// their uniqueness as well as tracking of available texture units on the GPU.
class TextureManager final : public InstanceManager<Texture, TextureManager> {
public:
    /// Computes a hash value for the given texture.
    /// @param texture The texture to hash.
    /// @return Hash value for the texture.
    static size_t Hash(const Texture& texture) {
        size_t seed = 0;
        CombineHashes(seed, std::hash<std::string>{}(texture.GetFilePath()));
        CombineHashes(seed, std::hash<GLenum>{}(texture.GetType()));
        return seed;
    }

    /// Compares two textures for equality.
    /// @param left The first texture to compare.
    /// @param right The second texture to compare.
    /// @return True if both textures are equivalent, false otherwise.
    static bool Equals(const Texture& left, const Texture& right) {
        return left.GetFilePath() == right.GetFilePath() &&
               left.GetType() == right.GetType();
    }

    /// Initializes the texture manager by querying available texture units.
    static void Initialize() {
        GLint maxUnits;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnits);
        textureUnits.resize(maxUnits, {GL_TEXTURE0, false});
        for (GLint i = 0; i < maxUnits; i++) {
            textureUnits[i].spot = GL_TEXTURE0 + i;
        }
    }

    /// Returns the GLenum representing an unassigned texture unit.
    /// @return GLenum value (-1) indicating no texture unit is assigned.
    static GLenum NoUnit() {
        return noUnit;
    }

    /// Retrieves a free GPU texture unit.
    /// @return An available GLenum representing a texture unit (e.g. GL_TEXTURE0).
    /// @throws std::runtime_error If no texture units are available.
    static GLenum GetFreeUnit() {
        for (auto& unit : textureUnits) { // NOLINT
            if (!unit.isOccupied) {
                unit.isOccupied = true;
                return unit.spot;
            }
        }
        throw std::runtime_error("No available texture units");
    }

    /// Releases a previously allocated texture unit.
    /// @param spot Reference to the texture unit to release.
    static void ReleaseUnit(GLenum& spot) {
        for (auto& textureUnit : textureUnits) { // NOLINT
            if (textureUnit.spot == spot) {
                textureUnit.isOccupied = false;
                spot = noUnit;
                return;
            }
        }
    }

private:
    /// Represents a single texture unit and its occupation status.
    struct TextureUnit {
        GLenum spot;      ///< The OpenGL texture unit (e.g. GL_TEXTURE0).
        bool isOccupied;  ///< Whether the texture unit is currently in use.
    };

    inline static std::vector<TextureUnit> textureUnits;      ///< List of all tracked texture units.
    static constexpr GLenum noUnit = static_cast<GLenum>(-1); ///< Special value indicating no texture unit.
};
