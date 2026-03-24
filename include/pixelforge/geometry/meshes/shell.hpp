#pragma once
#include <pixelforge/geometry/meshes/mesh.hpp>
#include <pixelforge/geometry/curves/curve.hpp>


namespace pixelforge::geometry {

    class Shell : public Mesh {
    public:
        explicit Shell(Curve*, Curve*, uint = 50, uint = 50);

    protected:
        Curve* outlineCurve_;
        Curve* beltCurve_;
        uint stackCount_;
        uint sectorCount_;
        void calculatePositions() override;
        void calculateIndices() override;
    };

} // namespace pixelforge::geometry
