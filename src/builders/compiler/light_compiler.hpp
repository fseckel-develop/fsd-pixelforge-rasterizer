#pragma once
#include <pixelforge/builders/specification/light_spec.hpp>
#include <pixelforge/scene/lighting/light.hpp>
#include <memory>


namespace pixelforge::scene::builders::compiler {

    /// Compiles a declarative LightSpec into a runtime Light object.
    /// @param spec The light specification to compile.
    /// @return Shared pointer to the constructed runtime light.
    std::shared_ptr<lighting::Light> buildLight(
        const pixelforge::builders::specification::LightSpec& spec
    );

} // namespace pixelforge::scene::builders::compiler
