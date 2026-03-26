#include <doctest.h>
#include <pixelforge/scene/animation/animation.hpp>
#include <pixelforge/scene/nodes/animation_node.hpp>
#include <pixelforge/scene/nodes/transform_node.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>
#include <memory>

using pixelforge::scene::animation::Animation;
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

    class FakeAnimation final : public Animation {
    public:
        Transform offset;

        FakeAnimation() :
            Animation(LOOP) {
        }

        [[nodiscard]] Transform getOffset() const override {
            return offset;
        }
    };

} // namespace


TEST_CASE("AnimationNode starts with dirty transform") {
    const auto node = std::make_shared<AnimationNode>("AnimNode");

    CHECK(node->isGlobalTransformDirty());
}

TEST_CASE("AnimationNode addAnimation marks transform dirty") {
    const auto node = std::make_shared<AnimationNode>("AnimNode");
    const auto animation = std::make_shared<FakeAnimation>();

    static_cast<void>(node->getGlobalTransform());
    CHECK_FALSE(node->isGlobalTransformDirty());

    node->addAnimation(animation);

    CHECK(node->isGlobalTransformDirty());
}

TEST_CASE("AnimationNode ignores null animation") {
    const auto node = std::make_shared<AnimationNode>("AnimNode");
    std::shared_ptr<Animation> animation = nullptr;

    static_cast<void>(node->getGlobalTransform());
    CHECK_FALSE(node->isGlobalTransformDirty());

    node->addAnimation(animation);

    CHECK_FALSE(node->isGlobalTransformDirty());
}

TEST_CASE("AnimationNode playAllAnimations forwards play to all animations") {
    const auto node = std::make_shared<AnimationNode>("AnimNode");
    const auto a = std::make_shared<FakeAnimation>();
    const auto b = std::make_shared<FakeAnimation>();

    node->addAnimation(a);
    node->addAnimation(b);

    a->stop();
    b->stop();

    CHECK_FALSE(a->isPlaying());
    CHECK_FALSE(b->isPlaying());

    node->playAllAnimations();

    CHECK(a->isPlaying());
    CHECK(b->isPlaying());
    CHECK_FALSE(a->isPaused());
    CHECK_FALSE(b->isPaused());
}

TEST_CASE("AnimationNode pauseAllAnimations forwards pause to all animations") {
    const auto node = std::make_shared<AnimationNode>("AnimNode");
    const auto a = std::make_shared<FakeAnimation>();
    const auto b = std::make_shared<FakeAnimation>();

    node->addAnimation(a);
    node->addAnimation(b);

    node->playAllAnimations();
    node->pauseAllAnimations();

    CHECK(a->isPaused());
    CHECK(b->isPaused());
}

TEST_CASE("AnimationNode resumeAllAnimations forwards resume to all animations") {
    const auto node = std::make_shared<AnimationNode>("AnimNode");
    const auto a = std::make_shared<FakeAnimation>();
    const auto b = std::make_shared<FakeAnimation>();

    node->addAnimation(a);
    node->addAnimation(b);

    node->pauseAllAnimations();
    REQUIRE(a->isPaused());
    REQUIRE(b->isPaused());

    node->resumeAllAnimations();

    CHECK_FALSE(a->isPaused());
    CHECK_FALSE(b->isPaused());
    CHECK(a->isPlaying());
    CHECK(b->isPlaying());
}

TEST_CASE("AnimationNode stopAllAnimations forwards stop to all animations") {
    const auto node = std::make_shared<AnimationNode>("AnimNode");
    const auto a = std::make_shared<FakeAnimation>();
    const auto b = std::make_shared<FakeAnimation>();

    node->addAnimation(a);
    node->addAnimation(b);

    node->stopAllAnimations();

    CHECK_FALSE(a->isPlaying());
    CHECK_FALSE(b->isPlaying());
    CHECK_FALSE(a->isPaused());
    CHECK_FALSE(b->isPaused());
}

TEST_CASE("AnimationNode resetAllAnimations preserves stopped state") {
    const auto node = std::make_shared<AnimationNode>("AnimNode");
    const auto a = std::make_shared<FakeAnimation>();
    const auto b = std::make_shared<FakeAnimation>();

    node->addAnimation(a);
    node->addAnimation(b);

    node->stopAllAnimations();
    CHECK_FALSE(a->isPlaying());
    CHECK_FALSE(b->isPlaying());

    node->resetAllAnimations();

    CHECK_FALSE(a->isPlaying());
    CHECK_FALSE(b->isPlaying());
    CHECK_FALSE(a->isPaused());
    CHECK_FALSE(b->isPaused());
}

TEST_CASE("AnimationNode updateAllAnimations marks node dirty") {
    const auto node = std::make_shared<AnimationNode>("AnimNode");
    const auto a = std::make_shared<FakeAnimation>();
    const auto b = std::make_shared<FakeAnimation>();

    node->addAnimation(a);
    node->addAnimation(b);

    static_cast<void>(node->getGlobalTransform());
    CHECK_FALSE(node->isGlobalTransformDirty());

    node->updateAllAnimations(0.25f);

    CHECK(node->isGlobalTransformDirty());
}

TEST_CASE("AnimationNode global transform includes local transform and animation offsets") {
    const auto node = std::make_shared<AnimationNode>("AnimNode");

    Transform local;
    local.setTranslation(1.0f, 2.0f, 3.0f);
    node->setTransform(local);

    const auto animation = std::make_shared<FakeAnimation>();
    animation->offset.setTranslation(4.0f, 5.0f, 6.0f);

    node->addAnimation(animation);

    const Transform& global = node->getGlobalTransform();
    checkVec3Approx(global.getTranslation(), glm::vec3(5.0f, 7.0f, 9.0f));
    CHECK_FALSE(node->isGlobalTransformDirty());
}

TEST_CASE("AnimationNode global transform includes ancestor transform local transform and animation offsets") {
    const auto parent = std::make_shared<TransformNode>("Parent");
    const auto node = std::make_shared<AnimationNode>("AnimNode");

    Transform parentLocal;
    parentLocal.setTranslation(10.0f, 0.0f, 0.0f);
    parent->setTransform(parentLocal);

    Transform local;
    local.setTranslation(1.0f, 2.0f, 3.0f);
    node->setTransform(local);

    const auto animation = std::make_shared<FakeAnimation>();
    animation->offset.setTranslation(4.0f, 0.0f, 0.0f);

    node->addAnimation(animation);
    parent->addChild(node);

    const Transform& global = node->getGlobalTransform();
    checkVec3Approx(global.getTranslation(), glm::vec3(15.0f, 2.0f, 3.0f));
}

TEST_CASE("AnimationNode updateSelfAndChildren refreshes dirty transform") {
    const auto node = std::make_shared<AnimationNode>("AnimNode");
    const auto animation = std::make_shared<FakeAnimation>();

    animation->offset.setTranslation(2.0f, 0.0f, 0.0f);
    node->addAnimation(animation);

    node->updateSelfAndChildren(0.5f);

    CHECK_FALSE(node->isGlobalTransformDirty());

    const Transform& global = node->getGlobalTransform();
    checkVec3Approx(global.getTranslation(), glm::vec3(2.0f, 0.0f, 0.0f));
}

TEST_CASE("AnimationNode removeAnimation removes its transform contribution") {
    const auto node = std::make_shared<AnimationNode>("AnimNode");
    const auto a = std::make_shared<FakeAnimation>();
    const auto b = std::make_shared<FakeAnimation>();

    a->offset.setTranslation(1.0f, 0.0f, 0.0f);
    b->offset.setTranslation(2.0f, 0.0f, 0.0f);

    node->addAnimation(a);
    node->addAnimation(b);

    checkVec3Approx(node->getGlobalTransform().getTranslation(), glm::vec3(3.0f, 0.0f, 0.0f));

    node->removeAnimation(a);
    checkVec3Approx(node->getGlobalTransform().getTranslation(), glm::vec3(2.0f, 0.0f, 0.0f));
}

TEST_CASE("AnimationNode updateAllAnimations invalidates cached global transform") {
    const auto node = std::make_shared<AnimationNode>("AnimNode");
    const auto animation = std::make_shared<FakeAnimation>();

    animation->offset.setTranslation(1.0f, 0.0f, 0.0f);
    node->addAnimation(animation);

    static_cast<void>(node->getGlobalTransform());
    CHECK_FALSE(node->isGlobalTransformDirty());

    node->updateAllAnimations(0.1f);

    CHECK(node->isGlobalTransformDirty());
}
