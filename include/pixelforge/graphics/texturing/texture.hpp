#pragma once
#include <GL/glew.h>
#include <string>


namespace pixelforge::graphics {

    /// Encapsulates the creation, binding, and parameter management for 2D textures in OpenGL.
    class Texture {
    public:
        /// Constructs a 2D texture from a file.
        /// @param fileName Path of the texture file relative to the texture directory.
        /// @param type OpenGL texture target (e.g. GL_TEXTURE_2D).
        /// @param format Internal and source pixel format (e.g. GL_RGBA).
        explicit Texture(const std::string& fileName, GLenum type = GL_TEXTURE_2D, GLint format = GL_RGBA);

        /// Sets texture wrapping mode for the S and T coordinates.
        /// @param wrapS Wrapping mode for texture coordinate S.
        /// @param wrapT Wrapping mode for texture coordinate T.
        virtual void setWrapping(GLint wrapS, GLint wrapT) const;

        /// Sets the texture filtering mode for minification and magnification.
        /// @param minFilter Filter used when shrinking the texture.
        /// @param magFilter Filter used when enlarging the texture.
        void setFiltering(GLint minFilter, GLint magFilter) const;

        /// Binds the texture to a free texture unit.
        /// @return The index of the texture unit this texture was bound to.
        int bindTexture();

        /// Unbinds the texture and releases its texture unit.
        void unbindTexture();

        /// Deletes the texture object from the GPU.
        void deleteTexture();

        /// Gets the file path from which the texture was loaded.
        /// @return Reference to the texture file path.
        [[nodiscard]] const std::string& getFilePath() const;

        /// Gets the OpenGL texture object ID.
        /// @return The texture object ID.
        [[nodiscard]] GLuint getID() const;

        /// Gets the OpenGL texture target type (e.g. GL_TEXTURE_2D).
        /// @return The texture target type.
        [[nodiscard]] GLenum getType() const;

        /// Gets the texture unit the texture is currently bound to.
        /// @return The OpenGL enum for the texture unit (e.g. GL_TEXTURE0).
        [[nodiscard]] GLenum getUnit() const;

        /// Virtual destructor for cleanup.
        virtual ~Texture() = default;

    protected:
        std::string filePath_; ///< Path to the texture file.
        GLuint textureID_;     ///< OpenGL texture object ID.
        GLenum textureType_;   ///< OpenGL texture target.
        GLenum textureUnit_;   ///< Active texture unit.

        /// Default constructor for derived classes.
        Texture();
    };

} // namespace pixelforge::graphics
