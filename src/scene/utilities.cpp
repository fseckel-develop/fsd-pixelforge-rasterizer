#include <pixelforge/scene/utilities.hpp>
#include <pixelforge/scene/nodes/group.hpp>
#include <pixelforge/scene/nodes/light_unit.hpp>
#include <pixelforge/scene/nodes/model.hpp>
#include <pixelforge/scene/nodes/scene.hpp>
#include <pixelforge/scene/nodes/render_unit.hpp>
#include <pixelforge/scene/nodes/transform_node.hpp>
#include <iostream>
#include <string>

#include "pixelforge/graphics/texturing/cube_map.hpp"


namespace pixelforge::scene::utilities {

    namespace {

        /// Creates a text indentation prefix for tree output.
        /// @param level Tree depth level.
        /// @return Indentation string.
        std::string indent(const int level) {
            return std::string(static_cast<std::size_t>(level * 2), ' ');
        }

        /// Returns a readable runtime type name for a scene node.
        /// @param node The node whose type should be identified.
        /// @return Human-readable node type name.
        std::string nodeTypeName(const std::shared_ptr<nodes::SceneNode>& node) {
            using namespace pixelforge::scene::nodes;

            if (std::dynamic_pointer_cast<Scene>(node)) return "Scene";
            if (std::dynamic_pointer_cast<Model>(node)) return "Model";
            if (std::dynamic_pointer_cast<RenderUnit>(node)) return "RenderUnit";
            if (std::dynamic_pointer_cast<LightUnit>(node)) return "LightUnit";
            if (std::dynamic_pointer_cast<Group>(node)) return "Group";
            if (std::dynamic_pointer_cast<TransformNode>(node)) return "TransformNode";
            return "SceneNode";
        }

        /// Recursively prints the subtree rooted at the given node.
        /// @param node Current node to print.
        /// @param level Current tree depth.
        void printSceneNodeRecursive(const std::shared_ptr<nodes::SceneNode>& node, const int level) {
            if (!node) {
                std::cout << indent(level) << "<null>\n";
                return;
            }
            std::cout
                << indent(level)
                << nodeTypeName(node)
                << " \"" << node->getName() << "\"";
            if (const auto parent = node->getParent()) {
                std::cout << "  [parent: " << parent->getName() << "]";
            } else {
                std::cout << "  [parent: <none>]";
            }
            std::cout << "  [children: " << node->getChildren().size() << "]\n";
            for (const auto& child : node->getChildren()) {
                printSceneNodeRecursive(child, level + 1);
            }
        }

        /// Prints a compact model summary.
        /// @param model The model to summarize.
        void printModelSummary(const std::shared_ptr<nodes::Model>& model) {
            if (!model) return;
            std::cout << "Model \"" << model->getName() << "\" summary:\n";
            std::cout << "  renderUnits: " << model->getRenderUnits().size() << "\n";
            for (const auto& renderUnit : model->getRenderUnits()) {
                std::cout << "    - " << (renderUnit ? renderUnit->getName() : "<null>") << "\n";
            }
            std::cout << "  lightUnits: " << model->getLightUnits().size() << "\n";
            for (const auto& lightUnit : model->getLightUnits()) {
                std::cout << "    - " << (lightUnit ? lightUnit->getName() : "<null>") << "\n";
            }
            if (const auto root = model->getRoot()) {
                std::cout << "  root: " << root->getName() << "\n";
            } else {
                std::cout << "  root: <none>\n";
            }
        }

    } // namespace


    void printSceneNodeTree(const std::shared_ptr<nodes::SceneNode>& node) {
        std::cout << "\n=== Scene Node Tree ===\n";
        printSceneNodeRecursive(node, 0);
        std::cout << "=== End Scene Node Tree ===\n\n";
    }

    void printSceneSummary(const std::shared_ptr<nodes::Scene>& scene) {
        if (!scene) {
            std::cout << "Scene is null\n";
            return;
        }
        std::cout << "\n=== Scene Summary ===\n";
        std::cout << "Scene: " << scene->getName() << "\n";
        std::cout << "Models: " << scene->getModels().size() << "\n";
        std::cout << "All LightUnits: " << scene->getAllLightUnits().size() << "\n";
        if (scene->hasCubeMap() && scene->getCubeMap()) {
            std::cout << "Sky box: " << scene->getCubeMap()->getFilePath() << "\n";
        } else {
            std::cout << "Sky box: <none>\n";
        }
        for (const auto& model : scene->getModels()) {
            printModelSummary(model);
        }
        std::cout << "=== End Scene Summary ===\n\n";
    }

    glm::vec3 validateDirection(const glm::vec3& direction, const std::string& context) {
        if (glm::length(direction) < 1e-6f) {
            throw std::invalid_argument("Invalid zero direction vector" + (context.empty() ? "" : " in " + context));
        }
        return glm::normalize(direction);
    }

} // namespace pixelforge::scene::utilities
