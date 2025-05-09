#include "FunctionCurve.h"
using namespace std; using namespace glm;


FunctionCurve::FunctionCurve(Function xF, Function yF, Function zF):
    Curve({}, 0, OPEN),
    xFunction(std::move(xF)),
    yFunction(std::move(yF)),
    zFunction(std::move(zF)) {
}


vec3 FunctionCurve::Position(const float t) const {
    vec3 result;
    result.x = xFunction(t);
    result.y = yFunction(t);
    result.z = zFunction(t);
    return result;
}


// TODO: Finding other forms of differentiation
vec3 FunctionCurve::Velocity(const float t) const {
    constexpr float h = 0.000001f;
    return (Position(t + h) - Position(t - h)) / (2.0f * h);
}


// TODO: Finding other forms of differentiation
vec3 FunctionCurve::Acceleration(const float t) const {
    constexpr float h = 0.000001f;
    return (Position(t + h) - 2.0f * Position(t) + Position(t - h)) / (h * h);
}
