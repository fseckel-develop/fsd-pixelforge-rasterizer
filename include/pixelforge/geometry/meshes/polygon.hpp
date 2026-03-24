#pragma once
#include <pixelforge/geometry/meshes/points.hpp>
#include <pixelforge/geometry/curves/curve.hpp>


namespace pixelforge::geometry {

    class Polygon final : public Points {
    public:
        explicit Polygon(const std::vector<glm::vec3>&);
        explicit Polygon(const Curve&, unsigned int = 1000, Curve::EvaluationType = Curve::POSITION);
    };

} // namespace pixelforge::geometry
