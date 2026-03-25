#pragma once
#include <pixelforge/builders/specification/scene_spec.hpp>
#include <pixelforge/scene/nodes/scene.hpp>
#include <memory>


namespace pixelforge::scene::builders::compiler {

    /// Compiles a declarative SceneSpec into a runtime Scene graph.
    /// @param spec The scene specification to compile.
    /// @return Shared pointer to the constructed runtime scene.
    std::shared_ptr<nodes::Scene> buildScene(
        const pixelforge::builders::specification::SceneSpec& spec
    );

} // namespace pixelforge::scene::builders::compiler
