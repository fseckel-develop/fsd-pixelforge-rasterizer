#pragma once
#include "Mesh.h"


class Cuboid : public Mesh {
public:
    explicit Cuboid(float = 1.0f, float = 0.5f, float = 0.7f);

private:
    float width;
    float height;
    float depth;
    void CalculatePositions() override;
    void CalculateIndices() override;
};
