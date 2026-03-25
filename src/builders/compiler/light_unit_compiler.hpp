#pragma once
#include <pixelforge/builders/specification/light_unit_spec.hpp>
#include <pixelforge/scene/nodes/light_unit.hpp>
#include <memory>


namespace pixelforge::scene::builders::compiler {

    /// Compiles a declarative LightUnitSpec into a runtime LightUnit object.
    /// @param spec The light unit specification to compile.
    /// @return Shared pointer to the constructed runtime light unit.
    std::shared_ptr<nodes::LightUnit> buildLightUnit(
        const pixelforge::builders::specification::LightUnitSpec& spec
    );

} // namespace pixelforge::scene::builders::compiler
