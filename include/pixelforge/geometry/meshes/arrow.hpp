#pragma once
#include <pixelforge/geometry/meshes/mesh.hpp>


namespace pixelforge::geometry {

    class Arrow final : public Mesh {
    public:
        explicit Arrow(float = 0.7f, float = 0.05f, uint = 30);

    private:
        float shaftLength_;
        float shaftRadius_;
        float tipLength_;
        float tipRadius_;
        uint sectorCount_;
        void calculatePositions() override;
        void calculateIndices() override;
    };

} // namespace pixelforge::geometry
