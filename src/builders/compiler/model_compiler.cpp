#include "builders/compiler/model_compiler.hpp"
#include "builders/compiler/light_unit_compiler.hpp"
#include "builders/compiler/render_unit_compiler.hpp"
#include "builders/compiler/transform_node_compiler_utils.hpp"
#include "managers/scene_node_manager.hpp"


namespace pixelforge::scene::builders::compiler {

    std::shared_ptr<nodes::Model> buildModel(const pixelforge::builders::specification::ModelSpec& spec) {
        auto model = std::static_pointer_cast<nodes::Model>(
            management::SceneNodeManager::registerNode(std::make_shared<nodes::Model>(spec.name))
        );
        if (!spec.parentName.empty()) {
            management::SceneNodeManager::setParent(model, spec.parentName);
        }
        applyTransformNodeSpec(model, spec);
        for (const auto& lightSpec : spec.lightUnits) {
            auto lightUnit = buildLightUnit(lightSpec);
            model->addLightUnit(lightUnit);
        }
        for (const auto& renderSpec : spec.renderUnits) {
            auto renderUnit = buildRenderUnit(renderSpec);
            model->addRenderUnit(renderUnit);
        }
        return model;
    }

} // namespace pixelforge::scene::builders::compiler
