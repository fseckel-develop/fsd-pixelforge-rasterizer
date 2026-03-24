#pragma once
#include <pixelforge/geometry/meshes/mesh.hpp>


namespace pixelforge::geometry {

    class Torus final : public Mesh {
    public:
        explicit Torus(float = 1.0f, float = 1.0f, uint = 50, uint = 50);

    private:
        float innerRadius_;
        float torusRadius_;
        uint stackCount_;
        uint sectorCount_;
        void calculatePositions() override;
        void calculateIndices() override;
    };

} // namespace pixelforge::geometry
