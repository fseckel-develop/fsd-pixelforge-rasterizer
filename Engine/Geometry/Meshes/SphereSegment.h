#pragma once
#include "Mesh.h"


class SphereSegment final : public Mesh {
public:
    explicit SphereSegment(float = 1.0f, float = 0.2f, float = 0.6f, uint = 50, uint = 50);

private:
    float radius;
    float segmentStart;
    float segmentEnd;
    uint stackCount;
    uint sectorCount;
    void CalculatePositions() override;
    void CalculateIndices() override;
};
