#pragma once
#include "Mesh.h"


class Cylinder final : public Mesh {
public:
    explicit Cylinder(float = 1.0f, float = 1.0f, uint = 50);

private:
    float radius;
    float height;
    uint sectorCount;
    void CalculatePositions() override;
    void CalculateIndices() override;
};
