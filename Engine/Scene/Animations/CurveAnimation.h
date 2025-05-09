#pragma once
#include "Animation.h"
#include <memory>
class Curve;


/// Extension of the Animation class which moves
/// object along an associated Curve over time.
class CurveAnimation final : public Animation {
public:
    /// Constructs a curve animation with the specified mode.
    /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
    explicit CurveAnimation(Mode mode);

    /// Constructs a curve animation with a specified curve and duration.
    /// @param curve A shared pointer to the curve to follow.
    /// @param duration The total duration of the animation.
    /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
    explicit CurveAnimation(const std::shared_ptr<Curve>& curve, float duration, Mode mode = BOUNCE);

    /// Sets the curve for the animation.
    /// @param curve Shared pointer to the curve to use as trajectory.
    void SetCurve(const std::shared_ptr<Curve>& curve);

    /// Retrieves the current curve used in the animation.
    /// @return Shared pointer to the curve being followed by the animation.
    [[nodiscard]] std::shared_ptr<Curve> GetCurve() const;

    /// Calculates the current offset based on the progress on the associated curve.
    /// @return Transform combining translation and rotation along the curve.
    [[nodiscard]] Transform GetOffset() override;

private:
    std::shared_ptr<Curve> curve; ///< The curve used for animated translation and rotation.
};
