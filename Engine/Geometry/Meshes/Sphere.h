#pragma once
#include "Mesh.h"


class Sphere final : public Mesh {
public:
    explicit Sphere(float = 0.5f, uint = 50, uint = 50);

private:
    float radius;
    uint stackCount;
    uint sectorCount;
    void CalculatePositions() override;
    void CalculateTextureCoordinates() override;
    void CalculateNormals() override;
    void CalculateIndices() override;
};