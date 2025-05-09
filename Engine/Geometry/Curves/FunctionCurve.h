#pragma once
#include "Curve.h"
#include <functional>
#include <GLM/glm.hpp>


/// Extension of the Curve class representing a function curve
/// with every coordinate being defined by a parametric function.
class FunctionCurve final : public Curve {
public:
    /// Alias for a scalar function of a float.
    using Function = std::function<float(float)>;

    /// Constructs a function curve using separate functions for x, y and z.
    /// @param xFunc Function representing x(t)
    /// @param yFunc Function representing y(t)
    /// @param zFunc Function representing z(t)
    FunctionCurve(Function xFunc, Function yFunc, Function zFunc);

private:
    Function xFunction; ///< Function defining the x-component
    Function yFunction; ///< Function defining the y-component
    Function zFunction; ///< Function defining the z-component

    /// @copydoc Curve::Position
    [[nodiscard]] glm::vec3 Position(float t) const override;

    /// @copydoc Curve::Velocity
    [[nodiscard]] glm::vec3 Velocity(float t) const override;

    /// @copydoc Curve::Acceleration
    [[nodiscard]] glm::vec3 Acceleration(float t) const override;
};
