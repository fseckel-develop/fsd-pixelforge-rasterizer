#pragma once
#include "Sphere.h"


class SphereSector final : public Mesh {
public:
    explicit SphereSector(float = 1.0f, float = 30.0f, uint = 50, uint = 50);

private:
    float radius;
    float openingAngle;
    uint stackCount;
    uint sectorCount;
    void CalculatePositions() override;
    void CalculateIndices() override;
};
