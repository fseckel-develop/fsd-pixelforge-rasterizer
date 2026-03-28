#include "builders/compiler/light_unit_compiler.hpp"
#include "builders/compiler/light_compiler.hpp"
#include "builders/compiler/transform_node_compiler_utils.hpp"
#include "managers/scene_node_manager.hpp"


namespace pixelforge::scene::builders::compiler {

    std::shared_ptr<nodes::LightUnit> buildLightUnit(const pixelforge::builders::specification::LightUnitSpec& spec) {
        auto lightUnit = std::static_pointer_cast<nodes::LightUnit>(
            management::SceneNodeManager::registerNode(std::make_shared<nodes::LightUnit>(spec.name))
        );
        if (!spec.parentName.empty()) {
            management::SceneNodeManager::setParent(lightUnit, spec.parentName);
        }
        if (spec.light) lightUnit->setLight(buildLight(*spec.light));
        if (spec.mesh) lightUnit->setMesh(spec.mesh);
        else if (!spec.meshFileName.empty()) {
            lightUnit->setMesh(std::make_shared<geometry::Mesh>(spec.meshFileName));
        }
        applyTransformNodeSpec(lightUnit, spec);
        return lightUnit;
    }

} // namespace pixelforge::scene::builders::compiler
