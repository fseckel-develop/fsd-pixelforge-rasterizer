#pragma once
#include <pixelforge/geometry/meshes/shell.hpp>


namespace pixelforge::geometry {

    class RoundedCube final : public Shell {
    public:
        explicit RoundedCube(float = 1.0f, uint = 50);

    private:
        float sideLength_;
        uint precision_;
    };

} // namespace pixelforge::geometry
