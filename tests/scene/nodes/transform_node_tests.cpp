#include <doctest.h>
#include <pixelforge/scene/nodes/transform_node.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>
#include <memory>

using pixelforge::scene::nodes::TransformNode;
using pixelforge::scene::transform::Transform;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
    }

    void checkMat4Approx(const glm::mat4& actual, const glm::mat4& expected) {
        for (int c = 0; c < 4; ++c) {
            for (int r = 0; r < 4; ++r) {
                CHECK(actual[c][r] == doctest::Approx(expected[c][r]).epsilon(EPS));
            }
        }
    }

} // namespace


TEST_CASE("TransformNode starts with dirty global transform") {
    const auto node = std::make_shared<TransformNode>("Node");

    CHECK(node->isGlobalTransformDirty());
}

TEST_CASE("TransformNode with no transform ancestor uses its local transform as global transform") {
    const auto node = std::make_shared<TransformNode>("Node");

    Transform local;
    local.setTranslation(1.0f, 2.0f, 3.0f);
    local.setScale(2.0f, 3.0f, 4.0f);

    node->setTransform(local);

    const Transform& global = node->getGlobalTransform();

    checkVec3Approx(global.getTranslation(), glm::vec3(1.0f, 2.0f, 3.0f));
    checkVec3Approx(global.getScale(), glm::vec3(2.0f, 3.0f, 4.0f));
    CHECK_FALSE(node->isGlobalTransformDirty());
}

TEST_CASE("TransformNode combines parent and child transforms into global transform") {
    const auto parent = std::make_shared<TransformNode>("Parent");
    const auto child = std::make_shared<TransformNode>("Child");

    Transform parentLocal;
    parentLocal.setTranslation(1.0f, 2.0f, 3.0f);
    parentLocal.setScale(2.0f, 2.0f, 2.0f);

    Transform childLocal;
    childLocal.setTranslation(4.0f, 5.0f, 6.0f);
    childLocal.setScale(3.0f, 4.0f, 5.0f);

    parent->setTransform(parentLocal);
    child->setTransform(childLocal);
    parent->addChild(child);

    const Transform& childGlobal = child->getGlobalTransform();

    // Current Transform::operator* behavior:
    // translation adds, scale multiplies component-wise
    checkVec3Approx(childGlobal.getTranslation(), glm::vec3(5.0f, 7.0f, 9.0f));
    checkVec3Approx(childGlobal.getScale(), glm::vec3(6.0f, 8.0f, 10.0f));

    CHECK_FALSE(child->isGlobalTransformDirty());
}

TEST_CASE("TransformNode markGlobalTransformDirty propagates to transform descendants") {
    const auto root = std::make_shared<TransformNode>("Root");
    const auto child = std::make_shared<TransformNode>("Child");
    const auto grandchild = std::make_shared<TransformNode>("Grandchild");

    root->addChild(child);
    child->addChild(grandchild);

    // Force computation once so flags become clean.
    static_cast<void>(root->getGlobalTransform());
    static_cast<void>(child->getGlobalTransform());
    static_cast<void>(grandchild->getGlobalTransform());

    CHECK_FALSE(root->isGlobalTransformDirty());
    CHECK_FALSE(child->isGlobalTransformDirty());
    CHECK_FALSE(grandchild->isGlobalTransformDirty());

    root->markGlobalTransformDirty();

    CHECK(root->isGlobalTransformDirty());
    CHECK(child->isGlobalTransformDirty());
    CHECK(grandchild->isGlobalTransformDirty());
}

TEST_CASE("TransformNode setTransform marks node dirty") {
    const auto node = std::make_shared<TransformNode>("Node");

    static_cast<void>(node->getGlobalTransform());
    CHECK_FALSE(node->isGlobalTransformDirty());

    Transform local;
    local.setTranslation(7.0f, 8.0f, 9.0f);

    node->setTransform(local);

    CHECK(node->isGlobalTransformDirty());
}

TEST_CASE("TransformNode parent transform change dirties child transform") {
    const auto parent = std::make_shared<TransformNode>("Parent");
    const auto child = std::make_shared<TransformNode>("Child");

    parent->addChild(child);

    static_cast<void>(parent->getGlobalTransform());
    static_cast<void>(child->getGlobalTransform());

    CHECK_FALSE(parent->isGlobalTransformDirty());
    CHECK_FALSE(child->isGlobalTransformDirty());

    Transform updated;
    updated.setTranslation(1.0f, 0.0f, 0.0f);
    parent->setTransform(updated);

    CHECK(parent->isGlobalTransformDirty());
    CHECK(child->isGlobalTransformDirty());
}

TEST_CASE("TransformNode node scale affects model matrix but not global transform") {
    const auto node = std::make_shared<TransformNode>("Node");

    Transform local;
    local.setTranslation(1.0f, 2.0f, 3.0f);
    node->setTransform(local);
    node->setNodeScale({2.0f, 3.0f, 4.0f});

    const Transform& global = node->getGlobalTransform();
    checkVec3Approx(global.getTranslation(), glm::vec3(1.0f, 2.0f, 3.0f));
    checkVec3Approx(global.getScale(), glm::vec3(1.0f, 1.0f, 1.0f));

    Transform expected;
    expected.setTranslation(1.0f, 2.0f, 3.0f);
    expected.setScale(2.0f, 3.0f, 4.0f);

    checkMat4Approx(node->getModelMatrix(), expected.toMatrix());
}

TEST_CASE("TransformNode uniform node scale affects model matrix") {
    const auto node = std::make_shared<TransformNode>("Node");

    node->setNodeScale(2.5f);

    Transform expected;
    expected.setScale(2.5f);

    checkMat4Approx(node->getModelMatrix(), expected.toMatrix());
}

TEST_CASE("TransformNode updateSelfAndChildren refreshes dirty transforms") {
    const auto root = std::make_shared<TransformNode>("Root");
    const auto child = std::make_shared<TransformNode>("Child");

    root->addChild(child);

    Transform rootLocal;
    rootLocal.setTranslation(1.0f, 2.0f, 3.0f);
    root->setTransform(rootLocal);

    CHECK(root->isGlobalTransformDirty());
    CHECK(child->isGlobalTransformDirty());

    root->updateSelfAndChildren(0.016f);

    CHECK_FALSE(root->isGlobalTransformDirty());
    CHECK_FALSE(child->isGlobalTransformDirty());

    const Transform& childGlobal = child->getGlobalTransform();
    checkVec3Approx(childGlobal.getTranslation(), glm::vec3(1.0f, 2.0f, 3.0f));
}
