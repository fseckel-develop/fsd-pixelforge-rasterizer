#pragma once
#include <pixelforge/geometry/meshes/mesh.hpp>


namespace pixelforge::geometry {

    class Frustum final : public Mesh {
    public:
        explicit Frustum(float = 1.0f, float = 0.5f, float = 1.0f, uint = 50);

    private:
        float radiusBase_;
        float radiusTop_;
        float height_;
        uint sectorCount_;
        void calculatePositions() override;
        void calculateIndices() override;
    };

} // namespace pixelforge::geometry
