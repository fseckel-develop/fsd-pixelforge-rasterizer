#pragma once
#include <pixelforge/geometry/meshes/sphere.hpp>


namespace pixelforge::geometry {

    class SphereSector final : public Mesh {
    public:
        explicit SphereSector(float = 1.0f, float = 30.0f, uint = 50, uint = 50);

    private:
        float radius_;
        float openingAngle_;
        uint stackCount_;
        uint sectorCount_;
        void calculatePositions() override;
        void calculateIndices() override;
    };

} // namespace pixelforge::geometry
