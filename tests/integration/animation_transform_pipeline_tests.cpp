#include <doctest.h>
#include <pixelforge/scene/animation/keyframe_animation.hpp>
#include <pixelforge/scene/animation/orbiting.hpp>
#include <pixelforge/scene/animation/rotation.hpp>
#include <pixelforge/scene/animation/scaling.hpp>
#include <pixelforge/scene/animation/translation.hpp>
#include <pixelforge/scene/nodes/animation_node.hpp>
#include <pixelforge/scene/nodes/transform_node.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <managers/animation_manager.hpp>
#include <managers/curve_manager.hpp>
#include <managers/light_manager.hpp>
#include <managers/material_manager.hpp>
#include <managers/mesh_manager.hpp>
#include <managers/scene_node_manager.hpp>
#include <managers/texture_manager.hpp>
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <memory>

using pixelforge::management::AnimationManager;
using pixelforge::management::CurveManager;
using pixelforge::management::LightManager;
using pixelforge::management::MaterialManager;
using pixelforge::management::MeshManager;
using pixelforge::management::SceneNodeManager;
using pixelforge::management::TextureManager;
using pixelforge::scene::animation::Animation;
using pixelforge::scene::animation::KeyframeAnimation;
using pixelforge::scene::animation::Orbiting;
using pixelforge::scene::animation::Rotation;
using pixelforge::scene::animation::Scaling;
using pixelforge::scene::animation::Translation;
using pixelforge::scene::nodes::AnimationNode;
using pixelforge::scene::nodes::TransformNode;
using pixelforge::scene::transform::Transform;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
    }

    void checkQuatApprox(const glm::quat& actual, const glm::quat& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
        CHECK(actual.w == doctest::Approx(expected.w).epsilon(EPS));
    }

    std::shared_ptr<pixelforge::scene::nodes::SceneNode> effectiveRoot(
        const std::shared_ptr<TransformNode>& node)
    {
        if (const auto root = node->getRoot()) return root;
        return node;
    }

    struct IntegrationManagersGuard {
        IntegrationManagersGuard() {
            SceneNodeManager::clear();
            AnimationManager::clear();
            CurveManager::clear();
            LightManager::clear();
            MaterialManager::clear();
            MeshManager::clear();
            TextureManager::clear();
        }

        ~IntegrationManagersGuard() {
            TextureManager::clear();
            MeshManager::clear();
            MaterialManager::clear();
            LightManager::clear();
            CurveManager::clear();
            AnimationManager::clear();
            SceneNodeManager::clear();
        }
    };

} // namespace


TEST_CASE("Adding animation inserts animation node above transform node") {
    IntegrationManagersGuard guard;

    auto node = std::make_shared<TransformNode>("NodeA");
    auto animation = std::make_shared<Translation>(
        Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 10.0f
    );

    node->addAnimation(animation);

    REQUIRE(node->getParent() != nullptr);
    CHECK(node->getParent()->getName() == "NodeA-Animation");

    const auto animationParent = std::dynamic_pointer_cast<AnimationNode>(node->getParent());
    REQUIRE(animationParent != nullptr);

    CHECK(node->getRoot() == animationParent);
}

TEST_CASE("Animated ancestor affects transform node global transform for translation") {
    IntegrationManagersGuard guard;

    auto node = std::make_shared<TransformNode>("NodeA");
    auto animation = std::make_shared<Translation>(
        Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 10.0f
    );

    node->addAnimation(animation);

    const auto root = effectiveRoot(node);
    root->updateSelfAndChildren(1.0f);

    const auto transform = node->getGlobalTransform();
    checkVec3Approx(transform.getTranslation(), glm::vec3(5.0f, 0.0f, 0.0f));
}

TEST_CASE("Animated ancestor affects transform node global transform for scaling") {
    IntegrationManagersGuard guard;

    auto node = std::make_shared<TransformNode>("NodeA");
    auto animation = std::make_shared<Scaling>(
        Animation::ONCE, 2.0f, glm::vec3(3.0f, 5.0f, 7.0f)
    );

    node->addAnimation(animation);

    const auto root = effectiveRoot(node);
    root->updateSelfAndChildren(1.0f);

    const auto transform = node->getGlobalTransform();
    checkVec3Approx(transform.getScale(), glm::vec3(2.0f, 3.0f, 4.0f));
}

TEST_CASE("Animated ancestor affects transform node global transform for rotation") {
    IntegrationManagersGuard guard;

    auto node = std::make_shared<TransformNode>("NodeA");
    auto animation = std::make_shared<Rotation>(
        Animation::ONCE, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f
    );

    node->addAnimation(animation);

    const auto root = effectiveRoot(node);
    root->updateSelfAndChildren(1.0f);

    const auto transform = node->getGlobalTransform();
    const auto expected = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    checkQuatApprox(transform.getRotation(), expected);
}

TEST_CASE("Paused animation does not advance through inserted animation node") {
    IntegrationManagersGuard guard;

    auto node = std::make_shared<TransformNode>("NodeA");
    auto animation = std::make_shared<Translation>(
        Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 10.0f
    );

    node->addAnimation(animation);
    const auto root = effectiveRoot(node);

    root->updateSelfAndChildren(0.5f);
    const auto beforePause = node->getGlobalTransform().getTranslation();

    animation->pause();
    root->updateSelfAndChildren(1.0f);

    const auto afterPause = node->getGlobalTransform().getTranslation();
    checkVec3Approx(afterPause, beforePause);
}

TEST_CASE("Stopped animation restarted with play influences transform from beginning") {
    IntegrationManagersGuard guard;

    auto node = std::make_shared<TransformNode>("NodeA");
    auto animation = std::make_shared<Translation>(
        Animation::ONCE, 2.0f, glm::vec3(0.0f, 0.0f, 1.0f), 8.0f
    );

    node->addAnimation(animation);
    const auto root = effectiveRoot(node);

    root->updateSelfAndChildren(2.0f);
    checkVec3Approx(node->getGlobalTransform().getTranslation(), glm::vec3(0.0f, 0.0f, 8.0f));

    animation->stop();
    animation->play();
    root->updateSelfAndChildren(1.0f);

    checkVec3Approx(node->getGlobalTransform().getTranslation(), glm::vec3(0.0f, 0.0f, 4.0f));
}

TEST_CASE("Parent transform composes with inserted animation ancestor and child transform") {
    IntegrationManagersGuard guard;

    auto parent = std::make_shared<TransformNode>("Parent");
    auto child = std::make_shared<TransformNode>("Child");

    Transform parentTransform;
    parentTransform.setTranslation(1.0f, 2.0f, 3.0f);
    parent->setTransform(parentTransform);

    parent->addChild(child);

    auto animation = std::make_shared<Translation>(
        Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 6.0f
    );
    child->addAnimation(animation);

    const auto root = effectiveRoot(child);
    root->updateSelfAndChildren(1.0f);

    const auto childGlobal = child->getGlobalTransform();
    checkVec3Approx(childGlobal.getTranslation(), glm::vec3(4.0f, 2.0f, 3.0f));
}

TEST_CASE("Multiple animations coexist through inserted animation node and affect model matrix") {
    IntegrationManagersGuard guard;

    auto node = std::make_shared<TransformNode>("NodeA");

    node->addAnimation(std::make_shared<Translation>(
        Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 10.0f
    ));
    node->addAnimation(std::make_shared<Scaling>(
        Animation::ONCE, 2.0f, glm::vec3(3.0f, 3.0f, 3.0f)
    ));
    node->addAnimation(std::make_shared<Rotation>(
        Animation::ONCE, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f
    ));

    const auto root = effectiveRoot(node);
    root->updateSelfAndChildren(1.0f);

    const auto matrix = node->getModelMatrix();
    CHECK_FALSE(matrix == glm::mat4(1.0f));

    const auto transform = node->getGlobalTransform();
    CHECK(transform.getTranslation().x > 0.0f);
    CHECK(transform.getScale().x > 1.0f);
}

TEST_CASE("Orbiting animation affects translation component through inserted animation node") {
    IntegrationManagersGuard guard;

    auto node = std::make_shared<TransformNode>("NodeA");
    auto animation = std::make_shared<Orbiting>(
        Animation::ONCE, 2.0f, 3.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f
    );

    node->addAnimation(animation);

    const auto root = effectiveRoot(node);
    root->updateSelfAndChildren(1.0f);

    const auto translation = node->getGlobalTransform().getTranslation();
    CHECK(glm::length(translation) == doctest::Approx(3.0f).epsilon(EPS));
}

TEST_CASE("Keyframe animation drives transform node translation through inserted animation node") {
    IntegrationManagersGuard guard;

    auto node = std::make_shared<TransformNode>("NodeA");
    auto animation = std::make_shared<KeyframeAnimation>(Animation::ONCE);

    Transform start;
    start.setTranslation(0.0f, 0.0f, 0.0f);

    Transform end;
    end.setTranslation(10.0f, 0.0f, 0.0f);

    animation->addKeyframe(0.0f, start);
    animation->addKeyframe(2.0f, end);

    node->addAnimation(animation);

    const auto root = effectiveRoot(node);
    root->updateSelfAndChildren(1.0f);

    const auto transform = node->getGlobalTransform();
    checkVec3Approx(transform.getTranslation(), glm::vec3(5.0f, 0.0f, 0.0f));
}

TEST_CASE("Animation insertion keeps transform evaluation stable across repeated updates") {
    IntegrationManagersGuard guard;

    auto node = std::make_shared<TransformNode>("NodeA");
    auto animation = std::make_shared<Translation>(
        Animation::ONCE, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 4.0f
    );

    node->addAnimation(animation);
    const auto root = effectiveRoot(node);

    CHECK_NOTHROW(static_cast<void>(node->getGlobalTransform()));

    root->updateSelfAndChildren(1.0f);
    const auto halfway = node->getGlobalTransform();
    checkVec3Approx(halfway.getTranslation(), glm::vec3(0.0f, 2.0f, 0.0f));

    root->updateSelfAndChildren(1.0f);
    const auto finished = node->getGlobalTransform();
    checkVec3Approx(finished.getTranslation(), glm::vec3(0.0f, 4.0f, 0.0f));
}
