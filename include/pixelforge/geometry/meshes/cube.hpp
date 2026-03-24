#pragma once
#include <pixelforge/geometry/meshes/cuboid.hpp>


namespace pixelforge::geometry {

    class Cube final : public Cuboid {
    public:
        explicit Cube(float = 1.0f);
    };

} // namespace pixelforge::geometry
