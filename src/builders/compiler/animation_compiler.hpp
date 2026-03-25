#pragma once
#include <pixelforge/builders/specification/animation_spec.hpp>
#include <pixelforge/scene/animation/animation.hpp>
#include <memory>


namespace pixelforge::scene::builders::compiler {

    /// Compiles a declarative AnimationSpec into a runtime Animation object.
    /// @param spec The animation specification to compile.
    /// @return Shared pointer to the constructed runtime animation.
    std::shared_ptr<animation::Animation> buildAnimation(
        const pixelforge::builders::specification::AnimationSpec& spec
    );

} // namespace pixelforge::scene::builders::compiler
