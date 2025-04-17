#pragma once
#include "Mesh.h"
#include "../Curves/Curve.h"


class Shell : public Mesh {
public:
    explicit Shell(Curve*, Curve*, uint = 50, uint = 50);

protected:
    Curve* outlineCurve;
    Curve* beltCurve;
    uint stackCount;
    uint sectorCount;
    void CalculatePositions() override;
    void CalculateIndices() override;
};
