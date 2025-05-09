#pragma once
#include <memory>
#include "../../Animations/Animation.h"


/// Concept to validate if a type implements a Build()
/// method which returns a shared pointer to an Animation.
template<typename T>
concept IsAnimationBuilder = requires(T t) {
    { t.Build() } -> std::convertible_to<std::shared_ptr<Animation>>;
};


/// Base class template for creating fluent-style builders for specific animation types.
/// @tparam AnimationT The specific Animation subclass to build.
/// @tparam DerivedBuilder The concrete builder class inheriting from this template.
template<typename AnimationT, typename DerivedBuilder>
class AnimationBuilder {
public:
    /// Starts the building process for an animation in a given mode.
    /// @param mode The animation mode to use (LOOP, ONCE, BOUNCE).
    explicit AnimationBuilder(const Animation::Mode mode) {
        animation = std::make_shared<AnimationT>(mode);
    }

    /// Sets the duration of the animation being built.
    /// @param duration The duration in seconds.
    /// @return Reference to the derived builder for fluent chaining.
    auto& withDuration(const float duration) {
        animation->SetDuration(duration);
        return static_cast<DerivedBuilder&>(*this);
    }

    /// Finalizes the build process and returns the animation.
    /// @return Shared pointer to the constructed animation.
    std::shared_ptr<AnimationT> Build() const {
        return animation;
    }

    /// Implicit conversion to allow the builder to be used as an animation.
    /// @return Shared pointer to the constructed animation.
    operator std::shared_ptr<AnimationT>() { // NOLINT
        return Build();
    }

protected:
    std::shared_ptr<AnimationT> animation; ///< The underlying animation being built.
};
