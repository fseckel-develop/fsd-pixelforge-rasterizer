#pragma once
#include <pixelforge/graphics/texturing/texture.hpp>
#include <vector>
#include <string>


namespace pixelforge::graphics {

    /// Extension of the Texture class to handle loading and
    /// configuring OpenGL cube maps using six image faces.
    class CubeMap final : public Texture {
    public:
        /// Constructs a CubeMap from six image file paths.
        /// @param baseName base name of the cube map file set.
        explicit CubeMap(const std::string& baseName);

        /// Sets texture wrapping mode for the R, S and T coordinates.
        /// @param wrapR Wrapping mode for the R coordinate.
        /// @param wrapS Wrapping mode for the S coordinate.
        /// @param wrapT Wrapping mode for the T coordinate.
        void setWrapping(GLint wrapR, GLint wrapS, GLint wrapT) const;

        /// Override of 2D wrapping setter to satisfy the base class.
        /// Cube maps require 3-axis wrapping configuration (R, S, T),
        /// so this method is intentionally left empty.
        void setWrapping(GLint, GLint) const override {}
    };

} // namespace pixelforge::graphics
