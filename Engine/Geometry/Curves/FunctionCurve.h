#pragma once
#include "Curve.h"
#include <functional>
#include <GLM/glm.hpp>
using namespace std;
using namespace glm;


class FunctionCurve final : public Curve {
public:
    using Function = function<float(float)>;
    FunctionCurve(Function, Function, Function);

private:
    Function xFunction, yFunction, zFunction;
    [[nodiscard]] vec3 Position(float t) const override;
    [[nodiscard]] vec3 Velocity(float t) const override;
    [[nodiscard]] vec3 Acceleration(float t) const override;
};
