#include <pixelforge/scene/nodes/model.hpp>
#include <pixelforge/scene/nodes/light_unit.hpp>
#include <pixelforge/scene/nodes/render_unit.hpp>


namespace pixelforge::scene::nodes {

    using std::vector;
    using std::string;
    using std::shared_ptr;


    Model::Model(const string& name):
        TransformNode(name) {
    }


    void Model::addLightUnit(const shared_ptr<LightUnit>& lightUnit) {
        lightUnits_.push_back(lightUnit);
        if (const auto root = lightUnit->getRoot()) {
            AddChild(root);
        } else {
            AddChild(lightUnit);
        }
    }


    void Model::addRenderUnit(const shared_ptr<RenderUnit>& renderUnit) {
        renderUnits_.push_back(renderUnit);
        if (const auto root = renderUnit->getRoot()) {
            AddChild(root);
        } else {
            AddChild(renderUnit);
        }
    }


    shared_ptr<LightUnit> Model::getLightUnitByName(const string& name) const {
        for (const auto& lightUnit : lightUnits_) {
            if (lightUnit && lightUnit->getName() == name) return lightUnit;
        }
        return nullptr;
    }


    shared_ptr<RenderUnit> Model::getRenderUnitByName(const string &name) const {
        for (const auto& renderUnit : renderUnits_) {
            if (renderUnit && renderUnit->getName() == name) return renderUnit;
        }
        return nullptr;
    }


    const vector<shared_ptr<LightUnit>>& Model::getLightUnits() const {
        return lightUnits_;
    }


    const vector<shared_ptr<RenderUnit>>& Model::getRenderUnits() const {
        return renderUnits_;
    }

} // namespace pixelforge::scene::nodes
