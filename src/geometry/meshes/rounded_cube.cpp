#include <pixelforge/geometry/meshes/rounded_cube.hpp>
#include <pixelforge/geometry/curves/bspline.hpp>
#include <pixelforge/utilities.hpp>


namespace pixelforge::geometry {

    using glm::vec3;


    RoundedCube::RoundedCube(const float sideLength, const uint precision):
        Shell(nullptr, nullptr) {
        this->sideLength_ = sideLength;
        this->precision_ = precision;
        std::vector controlPoints = {
            vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.5f, 0.0f), vec3(1.0f, 1.0f, 0.0f), vec3(0.5f, 1.0f, 0.0f),
            vec3(0.0f, 1.0f, 0.0f), vec3(-0.5f, 1.0f, 0.0f), vec3(-1.0f, 1.0f, 0.0f), vec3(-1.0f, 0.5f, 0.0f),
            vec3(-1.0f, 0.0f, 0.0f), vec3(-1.0f, -0.5f, 0.0f), vec3(-1.0f, -1.0f, 0.0f), vec3(-0.5f, -1.0f, 0.0f),
            vec3(0.0f, -1.0f, 0.0f), vec3(0.5f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f), vec3(1.0f, -0.5f, 0.0f),
        };
        beltCurve_ = new BSpline(controlPoints, 3, Curve::LOOP);
        const float s = sideLength / 2.0f;
        Utilities::scale(controlPoints, vec3(s));
        outlineCurve_ = new BSpline(controlPoints, 3, Curve::LOOP);
        outlineCurve_->setParameterRange(vec3(-s, 0.0f, 0.0f), vec3(s, 0.0f, 0.0f));
        stackCount_ = precision;
        sectorCount_ = precision;
        generateMeshData();
        Mesh::setupMesh();
    }

} // namespace pixelforge::geometry
