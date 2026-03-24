#include <pixelforge/geometry/curves/function_curve.hpp>


namespace pixelforge::geometry {

    using glm::vec3;


    FunctionCurve::FunctionCurve(Function xFunc, Function yFunc, Function zFunc):
        Curve({}, 0, OPEN),
        xFunction_(std::move(xFunc)),
        yFunction_(std::move(yFunc)),
        zFunction_(std::move(zFunc)) {
    }


    vec3 FunctionCurve::position(const float t) const {
        vec3 result;
        result.x = xFunction_(t);
        result.y = yFunction_(t);
        result.z = zFunction_(t);
        return result;
    }


    // TODO: Finding other forms of differentiation
    vec3 FunctionCurve::velocity(const float t) const {
        constexpr float h = 0.000001f;
        return (position(t + h) - position(t - h)) / (2.0f * h);
    }


    // TODO: Finding other forms of differentiation
    vec3 FunctionCurve::acceleration(const float t) const {
        constexpr float h = 0.000001f;
        return (position(t + h) - 2.0f * position(t) + position(t - h)) / (h * h);
    }

} // namespace pixelforge::geometry
