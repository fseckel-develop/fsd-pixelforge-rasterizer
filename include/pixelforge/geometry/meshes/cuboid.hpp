#pragma once
#include <pixelforge/geometry/meshes/mesh.hpp>


namespace pixelforge::geometry {

    class Cuboid : public Mesh {
    public:
        explicit Cuboid(float = 1.0f, float = 0.5f, float = 0.7f);

    private:
        float width_;
        float height_;
        float depth_;
        void calculatePositions() override;
        void calculateIndices() override;
    };

} // namespace pixelforge::geometry
