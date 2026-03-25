#pragma once
#include <pixelforge/builders/specification/model_spec.hpp>
#include <pixelforge/scene/nodes/model.hpp>
#include <memory>


namespace pixelforge::scene::builders::compiler {

    /// Compiles a declarative ModelSpec into a runtime Model object.
    /// @param spec The model specification to compile.
    /// @return Shared pointer to the constructed runtime model.
    std::shared_ptr<nodes::Model> buildModel(
        const pixelforge::builders::specification::ModelSpec& spec
    );

} // namespace pixelforge::scene::builders::compiler
