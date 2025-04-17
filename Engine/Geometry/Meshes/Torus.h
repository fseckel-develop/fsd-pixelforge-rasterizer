#pragma once
#include "Mesh.h"


class Torus final : public Mesh {
public:
    explicit Torus(float = 1.0f, float = 1.0f, uint = 50, uint = 50);

private:
    float innerRadius;
    float torusRadius;
    uint stackCount;
    uint sectorCount;
    void CalculatePositions() override;
    void CalculateIndices() override;
};
