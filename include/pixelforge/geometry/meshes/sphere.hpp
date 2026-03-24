#pragma once
#include <pixelforge/geometry/meshes/mesh.hpp>


namespace pixelforge::geometry {

    class Sphere final : public Mesh {
    public:
        explicit Sphere(float = 0.5f, uint = 50, uint = 50);

    private:
        float radius_;
        uint stackCount_;
        uint sectorCount_;
        void calculatePositions() override;
        void calculateTextureCoordinates() override;
        void calculateNormals() override;
        void calculateIndices() override;
    };

} // namespace pixelforge::geometry
