#pragma once
#include <pixelforge/geometry/meshes/mesh.hpp>


namespace pixelforge::geometry {

    class Cone final : public Mesh {
    public:
        static Cone ConeByRadius(float, float, uint = 50);
        static Cone ConeByAngle(float, float, uint = 50);

    private:
        explicit Cone(float, float, float, uint);
        float height_;
        float baseRadius_;
        float halfAngle_;
        uint sectorCount_;
        void calculatePositions() override;
        void calculateIndices() override;
    };

} // namespace pixelforge::geometry
