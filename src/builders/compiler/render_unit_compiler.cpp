#include "builders/compiler/render_unit_compiler.hpp"
#include "builders/compiler/transform_node_compiler_utils.hpp"
#include "managers/scene_node_manager.hpp"


namespace pixelforge::scene::builders::compiler {

    std::shared_ptr<nodes::RenderUnit> buildRenderUnit(const pixelforge::builders::specification::RenderUnitSpec& spec) {
        auto renderUnit = std::static_pointer_cast<nodes::RenderUnit>(
            management::SceneNodeManager::registerNode(std::make_shared<nodes::RenderUnit>(spec.name))
        );
        if (!spec.parentName.empty()) {
            management::SceneNodeManager::setParent(renderUnit, spec.parentName);
        }
        if (spec.mesh) renderUnit->setMesh(spec.mesh);
        else if (!spec.meshFileName.empty()) {
            renderUnit->setMesh(std::make_shared<geometry::Mesh>(spec.meshFileName));
        }
        if (spec.material) renderUnit->setMaterial(spec.material);
        else if (!spec.textureFileName.empty()) {
            renderUnit->setTexture(spec.textureFileName);
        }
        applyTransformNodeSpec(renderUnit, spec);
        return renderUnit;
    }

} // namespace pixelforge::scene::builders::compiler
