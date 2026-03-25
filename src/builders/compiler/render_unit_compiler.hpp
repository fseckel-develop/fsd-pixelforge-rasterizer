#pragma once
#include <pixelforge/builders/specification/render_unit_spec.hpp>
#include <pixelforge/scene/nodes/render_unit.hpp>
#include <memory>


namespace pixelforge::scene::builders::compiler {

    /// Compiles a declarative RenderUnitSpec into a runtime RenderUnit object.
    /// @param spec The render unit specification to compile.
    /// @return Shared pointer to the constructed runtime render unit.
    std::shared_ptr<nodes::RenderUnit> buildRenderUnit(
        const pixelforge::builders::specification::RenderUnitSpec& spec
    );

} // namespace pixelforge::scene::builders::compiler
