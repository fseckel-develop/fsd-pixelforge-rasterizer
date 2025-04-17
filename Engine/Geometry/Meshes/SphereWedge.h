#pragma once
#include "Mesh.h"


class SphereWedge final : public Mesh {
public:
    explicit SphereWedge(float = 1.0f, float = 60.0f, uint = 50, uint = 50);

private:
    float radius;
    float wedgeAngle;
    uint stackCount;
    uint sectorCount;
    void CalculatePositions() override;
    void CalculateIndices() override;
};
