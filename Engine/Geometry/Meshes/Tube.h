#pragma once
#include "Mesh.h"
#include "../Curves/Curve.h"


class Tube final : public Mesh {
public:
    explicit Tube(Curve*, Curve*, uint = 50, uint = 50);

private:
    Curve* spineCurve;
    Curve* beltCurve;
    glm::mat3 beltBase = glm::mat3();
    uint stackCount;
    uint sectorCount;
    void CalculatePositions() override;
    void CalculateIndices() override;
    void ComputeBeltBase();
    glm::vec3 ComputeBeltNormal() const;
    glm::vec3 ComputeBeltRadiant(glm::vec3) const;
    std::vector<glm::quat> ComputeBeltRotations() const;
};
