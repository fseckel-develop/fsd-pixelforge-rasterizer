#pragma once
#include <pixelforge/graphics/texturing/texture.hpp>
#include "managers/instance_manager.hpp"


namespace pixelforge::management {

    /// Extension of the InstanceManager class for managing texture instances, ensuring
    /// their uniqueness as well as tracking of available texture units on the GPU.
    class TextureManager final : public InstanceManager<graphics::Texture, TextureManager> {
    public:
        /// Computes a hash value for the given texture.
        /// @param texture The texture to hash.
        /// @return Hash value for the texture.
        static size_t hash(const graphics::Texture& texture) {
            size_t seed = 0;
            combineHashes(seed, std::hash<std::string>{}(texture.getFilePath()));
            combineHashes(seed, std::hash<GLenum>{}(texture.getType()));
            return seed;
        }

        /// Compares two textures for equality.
        /// @param left The first texture to compare.
        /// @param right The second texture to compare.
        /// @return True if both textures are equivalent, false otherwise.
        static bool equals(const graphics::Texture& left, const graphics::Texture& right) {
            return left.getFilePath() == right.getFilePath() &&
                   left.getType() == right.getType();
        }

        /// Initializes the texture manager by querying available texture units.
        static void initialize() {
            textureUnits_.clear(); // ensure fresh state
            GLint maxUnits;
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnits);
            textureUnits_.resize(maxUnits, {GL_TEXTURE0, false});
            for (GLint i = 0; i < maxUnits; i++) {
                textureUnits_[i].spot = GL_TEXTURE0 + i;
            }
        }

        /// Returns the GLenum representing an unassigned texture unit.
        /// @return GLenum value (-1) indicating no texture unit is assigned.
        static GLenum noUnit() {
            return noUnit_;
        }

        /// Retrieves a free GPU texture unit.
        /// @return An available GLenum representing a texture unit (e.g. GL_TEXTURE0).
        /// @throws std::runtime_error If no texture units are available.
        static GLenum getFreeUnit() {
            for (auto& unit : textureUnits_) { // NOLINT
                if (!unit.isOccupied) {
                    unit.isOccupied = true;
                    return unit.spot;
                }
            }
            throw std::runtime_error("No available texture units");
        }

        /// Releases a previously allocated texture unit.
        /// @param spot Reference to the texture unit to release.
        static void releaseUnit(GLenum& spot) {
            for (auto& textureUnit : textureUnits_) { // NOLINT
                if (textureUnit.spot == spot) {
                    textureUnit.isOccupied = false;
                    spot = noUnit_;
                    return;
                }
            }
        }

        /// Clears all tracked texture units entirely.
        /// Requires a subsequent call to initialize() before usage.
        static void clearUnits() {
            textureUnits_.clear();
        }

        /// Clears all managed textures and resets all texture unit state.
        /// After this call, initialize() must be called before using texture units again.
        static void clear() {
            InstanceManager::clear();
            clearUnits();
        }

    private:
        /// Represents a single texture unit and its occupation status.
        struct TextureUnit {
            GLenum spot;      ///< The OpenGL texture unit (e.g. GL_TEXTURE0).
            bool isOccupied;  ///< Whether the texture unit is currently in use.
        };

        inline static std::vector<TextureUnit> textureUnits_;      ///< List of all tracked texture units.
        static constexpr GLenum noUnit_ = static_cast<GLenum>(-1); ///< Special value indicating no texture unit.
    };

} // namespace pixelforge::management
