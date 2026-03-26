#include <doctest.h>
#include <managers/scene_node_manager.hpp>
#include <pixelforge/scene/nodes/scene_node.hpp>
#include <memory>
#include <stdexcept>
#include <string>

using pixelforge::management::SceneNodeManager;
using pixelforge::scene::nodes::SceneNode;


namespace {

    class TestSceneNode final : public SceneNode {
    public:
        explicit TestSceneNode(const std::string& name) :
            SceneNode(name) {
        }
    };

    struct SceneNodeManagerGuard {
        SceneNodeManagerGuard() { SceneNodeManager::clear(); }
        ~SceneNodeManagerGuard() { SceneNodeManager::clear(); }
    };

} // namespace


TEST_CASE("SceneNodeManager registerNode stores node by name") {
    SceneNodeManagerGuard guard;

    const auto node = std::make_shared<TestSceneNode>("NodeA");

    const auto registered = SceneNodeManager::registerNode(node);

    CHECK(registered == node);
    CHECK(SceneNodeManager::getNodeByName("NodeA") == node);
}

TEST_CASE("SceneNodeManager getNodeByName returns nullptr for unknown name") {
    SceneNodeManagerGuard guard;

    CHECK(SceneNodeManager::getNodeByName("Missing") == nullptr);
}

TEST_CASE("SceneNodeManager registerNode throws for duplicate name") {
    SceneNodeManagerGuard guard;

    const auto first = std::make_shared<TestSceneNode>("Duplicate");
    const auto second = std::make_shared<TestSceneNode>("Duplicate");

    SceneNodeManager::registerNode(first);

    CHECK_THROWS_AS(SceneNodeManager::registerNode(second), std::runtime_error);
}

TEST_CASE("SceneNodeManager setParent links registered parent and child") {
    SceneNodeManagerGuard guard;

    const auto parent = std::make_shared<TestSceneNode>("Parent");
    const auto child = std::make_shared<TestSceneNode>("Child");

    SceneNodeManager::registerNode(parent);
    SceneNodeManager::registerNode(child);

    SceneNodeManager::setParent(child, "Parent");

    REQUIRE(child->getParent() == parent);

    const auto children = parent->getChildren();
    REQUIRE(children.size() == 1);
    CHECK(children[0] == child);
}

TEST_CASE("SceneNodeManager setParent throws when setting node as its own parent") {
    SceneNodeManagerGuard guard;

    const auto node = std::make_shared<TestSceneNode>("Self");

    SceneNodeManager::registerNode(node);

    CHECK_THROWS_AS(SceneNodeManager::setParent(node, "Self"), std::runtime_error);
}

TEST_CASE("SceneNodeManager setParent throws when parent name is not registered") {
    SceneNodeManagerGuard guard;

    const auto child = std::make_shared<TestSceneNode>("Child");

    SceneNodeManager::registerNode(child);

    CHECK_THROWS_AS(SceneNodeManager::setParent(child, "MissingParent"), std::runtime_error);
}

TEST_CASE("SceneNodeManager clear removes all registered nodes") {
    SceneNodeManagerGuard guard;

    const auto nodeA = std::make_shared<TestSceneNode>("NodeA");
    const auto nodeB = std::make_shared<TestSceneNode>("NodeB");

    SceneNodeManager::registerNode(nodeA);
    SceneNodeManager::registerNode(nodeB);

    REQUIRE(SceneNodeManager::getNodeByName("NodeA") == nodeA);
    REQUIRE(SceneNodeManager::getNodeByName("NodeB") == nodeB);

    SceneNodeManager::clear();

    CHECK(SceneNodeManager::getNodeByName("NodeA") == nullptr);
    CHECK(SceneNodeManager::getNodeByName("NodeB") == nullptr);
}

TEST_CASE("SceneNodeManager can register same name again after clear") {
    SceneNodeManagerGuard guard;

    const auto first = std::make_shared<TestSceneNode>("Reusable");

    SceneNodeManager::registerNode(first);
    SceneNodeManager::clear();

    const auto second = std::make_shared<TestSceneNode>("Reusable");

    CHECK_NOTHROW(SceneNodeManager::registerNode(second));
    CHECK(SceneNodeManager::getNodeByName("Reusable") == second);
}

TEST_CASE("SceneNodeManager registerNode throws for null node") {
    SceneNodeManagerGuard guard;

    std::shared_ptr<SceneNode> node = nullptr;

    CHECK_THROWS_AS(SceneNodeManager::registerNode(node), std::runtime_error);
}

TEST_CASE("SceneNodeManager setParent throws for null node") {
    SceneNodeManagerGuard guard;

    const auto parent = std::make_shared<TestSceneNode>("Parent");
    SceneNodeManager::registerNode(parent);

    std::shared_ptr<SceneNode> node = nullptr;

    CHECK_THROWS_AS(SceneNodeManager::setParent(node, "Parent"), std::runtime_error);
}
