#pragma once
#include "Points.h"
#include "../Curves/Curve.h"


class Polygon final : public Points {
public:
    explicit Polygon(const std::vector<glm::vec3>&);
    explicit Polygon(const Curve&, unsigned int = 1000, Curve::EvaluationType = Curve::POSITION);
    void Render() const override;
};
