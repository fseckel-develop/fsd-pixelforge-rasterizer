#include <doctest.h>
#include <pixelforge/scene/nodes/scene_node.hpp>
#include <memory>
#include <string>
#include <vector>

using pixelforge::scene::nodes::SceneNode;


namespace {

    class TestSceneNode final : public SceneNode {
    public:
        explicit TestSceneNode(const std::string& name)
            : SceneNode(name) {}

        int updateCount = 0;
        std::vector<float> observedDeltaTimes;

    protected:
        void updateSelf(float deltaTime) override {
            ++updateCount;
            observedDeltaTimes.push_back(deltaTime);
        }
    };

} // namespace


TEST_CASE("SceneNode constructor assigns non-empty name and unique id") {
    const auto a = std::make_shared<TestSceneNode>("A");
    const auto b = std::make_shared<TestSceneNode>("B");

    CHECK(a->getName() == "A");
    CHECK(b->getName() == "B");
    CHECK(a->getID() != b->getID());
}

TEST_CASE("SceneNode empty name falls back to generated name") {
    const auto node = std::make_shared<TestSceneNode>("");

    CHECK_FALSE(node->getName().empty());
    CHECK(node->getName().starts_with("SceneNode_"));
}

TEST_CASE("SceneNode setName updates name") {
    const auto node = std::make_shared<TestSceneNode>("OldName");

    node->setName("NewName");

    CHECK(node->getName() == "NewName");
}

TEST_CASE("SceneNode addChild establishes parent-child relationship") {
    const auto parent = std::make_shared<TestSceneNode>("Parent");
    const auto child = std::make_shared<TestSceneNode>("Child");

    parent->addChild(child);

    REQUIRE(parent->getChildren().size() == 1);
    CHECK(parent->getChildren()[0] == child);
    CHECK(child->getParent() == parent);
}

TEST_CASE("SceneNode addChild ignores null child") {
    const auto parent = std::make_shared<TestSceneNode>("Parent");
    std::shared_ptr<TestSceneNode> child = nullptr;

    parent->addChild(child);

    CHECK(parent->getChildren().empty());
}

TEST_CASE("SceneNode addChild ignores self as child") {
    const auto node = std::make_shared<TestSceneNode>("Node");

    node->addChild(node);

    CHECK(node->getChildren().empty());
    CHECK(node->getParent() == nullptr);
}

TEST_CASE("SceneNode addChild does not duplicate existing child") {
    const auto parent = std::make_shared<TestSceneNode>("Parent");
    const auto child = std::make_shared<TestSceneNode>("Child");

    parent->addChild(child);
    parent->addChild(child);

    REQUIRE(parent->getChildren().size() == 1);
    CHECK(child->getParent() == parent);
}

TEST_CASE("SceneNode removeChild detaches child and clears parent") {
    const auto parent = std::make_shared<TestSceneNode>("Parent");
    const auto child = std::make_shared<TestSceneNode>("Child");

    parent->addChild(child);
    REQUIRE(parent->getChildren().size() == 1);

    parent->removeChild(child);

    CHECK(parent->getChildren().empty());
    CHECK(child->getParent() == nullptr);
}

TEST_CASE("SceneNode removeChild ignores null child") {
    const auto parent = std::make_shared<TestSceneNode>("Parent");
    std::shared_ptr<TestSceneNode> child = nullptr;

    parent->removeChild(child);

    CHECK(parent->getChildren().empty());
}

TEST_CASE("SceneNode addChild reparents child from old parent") {
    const auto parentA = std::make_shared<TestSceneNode>("ParentA");
    const auto parentB = std::make_shared<TestSceneNode>("ParentB");
    const auto child = std::make_shared<TestSceneNode>("Child");

    parentA->addChild(child);
    REQUIRE(parentA->getChildren().size() == 1);
    CHECK(child->getParent() == parentA);

    parentB->addChild(child);

    CHECK(parentA->getChildren().empty());
    REQUIRE(parentB->getChildren().size() == 1);
    CHECK(parentB->getChildren()[0] == child);
    CHECK(child->getParent() == parentB);
}

TEST_CASE("SceneNode getRoot returns self for root node") {
    const auto root = std::make_shared<TestSceneNode>("Root");

    CHECK(root->getRoot() == root);
}

TEST_CASE("SceneNode getRoot returns top-most ancestor") {
    const auto root = std::make_shared<TestSceneNode>("Root");
    const auto middle = std::make_shared<TestSceneNode>("Middle");
    const auto leaf = std::make_shared<TestSceneNode>("Leaf");

    root->addChild(middle);
    middle->addChild(leaf);

    CHECK(middle->getRoot() == root);
    CHECK(leaf->getRoot() == root);
}

TEST_CASE("SceneNode prevents creating cycles by adding ancestor as child") {
    const auto root = std::make_shared<TestSceneNode>("Root");
    const auto child = std::make_shared<TestSceneNode>("Child");
    const auto grandchild = std::make_shared<TestSceneNode>("Grandchild");

    root->addChild(child);
    child->addChild(grandchild);

    grandchild->addChild(root);

    CHECK(root->getParent() == nullptr);
    REQUIRE(root->getChildren().size() == 1);
    CHECK(root->getChildren()[0] == child);
    REQUIRE(child->getChildren().size() == 1);
    CHECK(child->getChildren()[0] == grandchild);
    CHECK(grandchild->getChildren().empty());
}

TEST_CASE("SceneNode insertNodeAbove inserts node between current node and old parent") {
    const auto root = std::make_shared<TestSceneNode>("Root");
    const auto child = std::make_shared<TestSceneNode>("Child");
    const auto inserted = std::make_shared<TestSceneNode>("Inserted");

    root->addChild(child);
    REQUIRE(root->getChildren().size() == 1);
    CHECK(child->getParent() == root);

    child->insertNodeAbove(inserted);

    REQUIRE(root->getChildren().size() == 1);
    CHECK(root->getChildren()[0] == inserted);
    CHECK(inserted->getParent() == root);

    REQUIRE(inserted->getChildren().size() == 1);
    CHECK(inserted->getChildren()[0] == child);
    CHECK(child->getParent() == inserted);
}

TEST_CASE("SceneNode insertNodeAbove on root makes inserted node new root") {
    const auto root = std::make_shared<TestSceneNode>("Root");
    const auto inserted = std::make_shared<TestSceneNode>("Inserted");

    root->insertNodeAbove(inserted);

    CHECK(root->getParent() == inserted);
    REQUIRE(inserted->getChildren().size() == 1);
    CHECK(inserted->getChildren()[0] == root);
    CHECK(root->getRoot() == inserted);
}

TEST_CASE("SceneNode updateSelfAndChildren updates whole subtree once") {
    const auto root = std::make_shared<TestSceneNode>("Root");
    const auto childA = std::make_shared<TestSceneNode>("ChildA");
    const auto childB = std::make_shared<TestSceneNode>("ChildB");
    const auto grandchild = std::make_shared<TestSceneNode>("Grandchild");

    root->addChild(childA);
    root->addChild(childB);
    childA->addChild(grandchild);

    root->updateSelfAndChildren(0.25f);

    CHECK(root->updateCount == 1);
    CHECK(childA->updateCount == 1);
    CHECK(childB->updateCount == 1);
    CHECK(grandchild->updateCount == 1);

    REQUIRE(root->observedDeltaTimes.size() == 1);
    REQUIRE(childA->observedDeltaTimes.size() == 1);
    REQUIRE(childB->observedDeltaTimes.size() == 1);
    REQUIRE(grandchild->observedDeltaTimes.size() == 1);

    CHECK(root->observedDeltaTimes[0] == doctest::Approx(0.25f));
    CHECK(childA->observedDeltaTimes[0] == doctest::Approx(0.25f));
    CHECK(childB->observedDeltaTimes[0] == doctest::Approx(0.25f));
    CHECK(grandchild->observedDeltaTimes[0] == doctest::Approx(0.25f));
}
