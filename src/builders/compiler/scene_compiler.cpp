#include "builders/compiler/scene_compiler.hpp"
#include "builders/compiler/light_unit_compiler.hpp"
#include "builders/compiler/model_compiler.hpp"
#include "managers/scene_node_manager.hpp"


namespace pixelforge::scene::builders::compiler {

    std::shared_ptr<nodes::Scene> buildScene(const pixelforge::builders::specification::SceneSpec& spec) {
        management::SceneNodeManager::clear();
        auto scene = std::static_pointer_cast<nodes::Scene>(
            management::SceneNodeManager::registerNode(std::make_shared<nodes::Scene>(spec.name))
        );
        for (const auto& lightSpec : spec.lightUnits) {
            auto lightUnit = buildLightUnit(lightSpec);
            scene->addLightUnit(lightUnit);
        }
        for (const auto& modelSpec : spec.models) {
            auto model = buildModel(modelSpec);
            scene->addModel(model);
        }
        return scene;
    }

} // namespace pixelforge::scene::builders::compiler
