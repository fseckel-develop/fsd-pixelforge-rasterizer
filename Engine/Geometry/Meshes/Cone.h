#pragma once
#include "Mesh.h"


class Cone final : public Mesh {
public:
    static Cone ConeByRadius(float, float, uint = 50);
    static Cone ConeByAngle(float, float, uint = 50);

private:
    explicit Cone(float, float, float, uint);
    float height;
    float baseRadius;
    float halfAngle;
    uint sectorCount;
    void CalculatePositions() override;
    void CalculateIndices() override;
};
