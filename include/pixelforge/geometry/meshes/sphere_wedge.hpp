#pragma once
#include <pixelforge/geometry/meshes/mesh.hpp>


namespace pixelforge::geometry {

    class SphereWedge final : public Mesh {
    public:
        explicit SphereWedge(float = 1.0f, float = 60.0f, uint = 50, uint = 50);

    private:
        float radius_;
        float wedgeAngle_;
        uint stackCount_;
        uint sectorCount_;
        void calculatePositions() override;
        void calculateIndices() override;
    };

} // namespace pixelforge::geometry
