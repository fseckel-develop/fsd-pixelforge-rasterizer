#pragma once
#include <pixelforge/geometry/meshes/mesh.hpp>


namespace pixelforge::geometry {

    /// Flat double-sided ring mesh in the XZ plane.
    /// Useful for objects such as Saturn's ring.
    class Annulus final : public Mesh {
    public:
        /// Constructs an annulus mesh.
        /// @param innerRadius Inner radius of the ring.
        /// @param outerRadius Outer radius of the ring.
        /// @param segmentCount Number of radial segments.
        explicit Annulus(float innerRadius = 0.5f, float outerRadius = 1.0f, unsigned int segmentCount = 128);

    protected:
        /// Computes vertex positions for both top and bottom sides.
        void calculatePositions() override;

        /// Computes normals for both top and bottom sides.
        void calculateNormals() override;

        /// Computes texture coordinates in a radial planar mapping.
        void calculateTextureCoordinates() override;

        /// Computes triangle indices for both top and bottom sides.
        void calculateIndices() override;

    private:
        float innerRadius_;          ///< Inner radius of the ring.
        float outerRadius_;          ///< Outer radius of the ring.
        unsigned int segmentCount_;  ///< Number of ring segments.
    };

} // namespace pixelforge::geometry
