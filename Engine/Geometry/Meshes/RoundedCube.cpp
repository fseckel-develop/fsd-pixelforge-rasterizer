#include "RoundedCube.h"
#include "../Curves/BSpline.h"
#include "../../Utilities.h"


RoundedCube::RoundedCube(const float sideLength, const uint precision):
    Shell(nullptr, nullptr) {
    this->sideLength = sideLength;
    this->precision = precision;
    std::vector controlPoints = {
        vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.5f, 0.0f), vec3(1.0f, 1.0f, 0.0f), vec3(0.5f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f), vec3(-0.5f, 1.0f, 0.0f), vec3(-1.0f, 1.0f, 0.0f), vec3(-1.0f, 0.5f, 0.0f),
        vec3(-1.0f, 0.0f, 0.0f), vec3(-1.0f, -0.5f, 0.0f), vec3(-1.0f, -1.0f, 0.0f), vec3(-0.5f, -1.0f, 0.0f),
        vec3(0.0f, -1.0f, 0.0f), vec3(0.5f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f), vec3(1.0f, -0.5f, 0.0f),
    };
    beltCurve = new BSpline(controlPoints, 3, Curve::LOOP);
    const float s = sideLength / 2.0f;
    Utilities::Scale(controlPoints, vec3(s));
    outlineCurve = new BSpline(controlPoints, 3, Curve::LOOP);
    outlineCurve->SetParameterRange(vec3(-s, 0.0f, 0.0f), vec3(s, 0.0f, 0.0f));
    stackCount = precision;
    sectorCount = precision;
    GenerateMeshData();
    Mesh::SetupMesh();
}
