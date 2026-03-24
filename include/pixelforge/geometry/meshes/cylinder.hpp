#pragma once
#include <pixelforge/geometry/meshes/mesh.hpp>


namespace pixelforge::geometry {

    class Cylinder final : public Mesh {
    public:
        explicit Cylinder(float = 1.0f, float = 1.0f, uint = 50);

    private:
        float radius_;
        float height_;
        uint sectorCount_;
        void calculatePositions() override;
        void calculateIndices() override;
    };

} // namespace pixelforge::geometry
