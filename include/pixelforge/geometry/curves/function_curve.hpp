#pragma once
#include <pixelforge/geometry/curves/curve.hpp>
#include <GLM/glm.hpp>
#include <functional>


namespace pixelforge::geometry {

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
        Function xFunction_; ///< Function defining the x-component
        Function yFunction_; ///< Function defining the y-component
        Function zFunction_; ///< Function defining the z-component

        /// @copydoc Curve::position
        [[nodiscard]] glm::vec3 position(float t) const override;

        /// @copydoc Curve::velocity
        [[nodiscard]] glm::vec3 velocity(float t) const override;

        /// @copydoc Curve::acceleration
        [[nodiscard]] glm::vec3 acceleration(float t) const override;
    };

} // namespace pixelforge::geometry
