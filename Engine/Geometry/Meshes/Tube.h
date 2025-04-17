#pragma once
#include "Mesh.h"
#include "../Curves/Curve.h"


class Tube final : public Mesh {
public:
    explicit Tube(Curve*, Curve*, uint = 50, uint = 50);

private:
    Curve* spineCurve;
    Curve* beltCurve;
    mat3 beltBase = mat3();
    uint stackCount;
    uint sectorCount;
    void CalculatePositions() override;
    void CalculateIndices() override;
    void ComputeBeltBase();
    vec3 ComputeBeltNormal() const;
    vec3 ComputeBeltRadiant(vec3) const;
    vector<quat> ComputeBeltRotations() const;
};
