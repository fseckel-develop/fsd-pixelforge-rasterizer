#pragma once
#include "Mesh.h"


class Frustum final : public Mesh {
public:
    explicit Frustum(float = 1.0f, float = 0.5f, float = 1.0f, uint = 50);

private:
    float radiusBase;
    float radiusTop;
    float height;
    uint sectorCount;
    void CalculatePositions() override;
    void CalculateIndices() override;
};
