#pragma once
#include "builders/compiler/animation_compiler.hpp"
#include <pixelforge/scene/nodes/transform_node.hpp>


namespace pixelforge::scene::builders::compiler {

    /// Applies transform-related specification data to a runtime TransformNode.
    /// @tparam SpecT A specification type exposing transform, uniformNodeScale,
    /// nodeScale, and animations members.
    /// @param node The runtime transform node to configure.
    /// @param spec The specification whose transform-related data is applied.
    template<typename SpecT>
    void applyTransformNodeSpec(
        const std::shared_ptr<nodes::TransformNode>& node,
        const SpecT& spec
    ) {
        if (spec.transform) node->setTransform(*spec.transform);
        if (spec.uniformNodeScale) node->setNodeScale(*spec.uniformNodeScale);
        else if (spec.nodeScale) node->setNodeScale(*spec.nodeScale);

        for (const auto& animation : spec.animations) {
            node->addAnimation(buildAnimation(animation));
        }
    }

} // namespace pixelforge::scene::builders::compiler
