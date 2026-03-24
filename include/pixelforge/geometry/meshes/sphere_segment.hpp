#pragma once
#include <pixelforge/geometry/meshes/mesh.hpp>


namespace pixelforge::geometry {

    class SphereSegment final : public Mesh {
    public:
        explicit SphereSegment(float = 1.0f, float = 0.2f, float = 0.6f, uint = 50, uint = 50);

    private:
        float radius_;
        float segmentStart_;
        float segmentEnd_;
        uint stackCount_;
        uint sectorCount_;
        void calculatePositions() override;
        void calculateIndices() override;
    };

} // namespace pixelforge::geometry
