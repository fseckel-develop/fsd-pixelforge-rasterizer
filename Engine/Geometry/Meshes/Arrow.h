#pragma once
#include "Mesh.h"


class Arrow final : public Mesh {
public:
    explicit Arrow(float = 0.7f, float = 0.05f, uint = 30);

private:
    float shaftLength;
    float shaftRadius;
    float tipLength;
    float tipRadius;
    uint sectorCount;
    void CalculatePositions() override;
    void CalculateIndices() override;
};
