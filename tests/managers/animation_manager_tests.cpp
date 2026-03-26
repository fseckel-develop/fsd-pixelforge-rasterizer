#include <doctest.h>
#include <managers/animation_manager.hpp>
#include <pixelforge/geometry/curves/bezier_curve.hpp>
#include <pixelforge/scene/animation/curve_animation.hpp>
#include <pixelforge/scene/animation/keyframe_animation.hpp>
#include <pixelforge/scene/animation/orbiting.hpp>
#include <pixelforge/scene/animation/rotation.hpp>
#include <pixelforge/scene/animation/scaling.hpp>
#include <pixelforge/scene/animation/translation.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>
#include <memory>
#include <vector>

using pixelforge::geometry::BezierCurve;
using pixelforge::management::AnimationManager;
using pixelforge::scene::animation::Animation;
using pixelforge::scene::animation::CurveAnimation;
using pixelforge::scene::animation::KeyframeAnimation;
using pixelforge::scene::animation::Orbiting;
using pixelforge::scene::animation::Rotation;
using pixelforge::scene::animation::Scaling;
using pixelforge::scene::animation::Translation;
using pixelforge::scene::transform::Transform;


namespace {

    std::shared_ptr<BezierCurve> makeCurveA() {
        return std::make_shared<BezierCurve>(
            std::vector<glm::vec3>{
                {0.0f, 0.0f, 0.0f},
                {1.0f, 2.0f, 0.0f},
                {3.0f, 2.0f, 0.0f},
                {4.0f, 0.0f, 0.0f}
            }
        );
    }

    std::shared_ptr<BezierCurve> makeCurveB() {
        return std::make_shared<BezierCurve>(
            std::vector<glm::vec3>{
                {0.0f, 0.0f, 0.0f},
                {2.0f, 2.0f, 0.0f},
                {3.0f, 2.0f, 0.0f},
                {4.0f, 0.0f, 0.0f}
            }
        );
    }

    Transform makeTransform(const glm::vec3& translation,
                            const glm::vec3& eulerDegrees = glm::vec3(0.0f),
                            const glm::vec3& scale = glm::vec3(1.0f)) {
        Transform t;
        t.setTranslation(translation);
        t.setRotation(eulerDegrees);
        t.setScale(scale);
        return t;
    }

    struct AnimationManagerGuard {
        AnimationManagerGuard() { AnimationManager::clear(); }
        ~AnimationManagerGuard() { AnimationManager::clear(); }
    };

} // namespace


TEST_CASE("AnimationManager returns same pointer for same animation instance") {
    AnimationManagerGuard guard;

    const auto animation = std::make_shared<Translation>(
        Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 5.0f
    );

    const auto managedA = AnimationManager::getOrCreate(animation);
    const auto managedB = AnimationManager::getOrCreate(animation);

    CHECK(managedA == animation);
    CHECK(managedB == animation);
    CHECK(managedA == managedB);
}

TEST_CASE("AnimationManager canonicalizes equivalent Translation animations") {
    AnimationManagerGuard guard;

    const auto animationA = std::make_shared<Translation>(
        Animation::ONCE, 2.0f, glm::vec3(2.0f, 0.0f, 0.0f), 5.0f
    );
    const auto animationB = std::make_shared<Translation>(
        Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 5.0f
    );

    const auto managedA = AnimationManager::getOrCreate(animationA);
    const auto managedB = AnimationManager::getOrCreate(animationB);

    CHECK(managedA == managedB);
}

TEST_CASE("AnimationManager distinguishes Translation animations with different direction") {
    AnimationManagerGuard guard;

    const auto animationA = std::make_shared<Translation>(
        Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 5.0f
    );
    const auto animationB = std::make_shared<Translation>(
        Animation::ONCE, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 5.0f
    );

    const auto managedA = AnimationManager::getOrCreate(animationA);
    const auto managedB = AnimationManager::getOrCreate(animationB);

    CHECK(managedA != managedB);
}

TEST_CASE("AnimationManager distinguishes Translation animations with different distance") {
    AnimationManagerGuard guard;

    const auto animationA = std::make_shared<Translation>(
        Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 5.0f
    );
    const auto animationB = std::make_shared<Translation>(
        Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 7.0f
    );

    const auto managedA = AnimationManager::getOrCreate(animationA);
    const auto managedB = AnimationManager::getOrCreate(animationB);

    CHECK(managedA != managedB);
}

TEST_CASE("AnimationManager canonicalizes equivalent Rotation animations") {
    AnimationManagerGuard guard;

    const auto animationA = std::make_shared<Rotation>(
        Animation::LOOP, 3.0f, glm::vec3(0.0f, 2.0f, 0.0f), 180.0f
    );
    const auto animationB = std::make_shared<Rotation>(
        Animation::LOOP, 3.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f
    );

    const auto managedA = AnimationManager::getOrCreate(animationA);
    const auto managedB = AnimationManager::getOrCreate(animationB);

    CHECK(managedA == managedB);
}

TEST_CASE("AnimationManager distinguishes Rotation animations with different angle") {
    AnimationManagerGuard guard;

    const auto animationA = std::make_shared<Rotation>(
        Animation::LOOP, 3.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f
    );
    const auto animationB = std::make_shared<Rotation>(
        Animation::LOOP, 3.0f, glm::vec3(0.0f, 1.0f, 0.0f), 90.0f
    );

    const auto managedA = AnimationManager::getOrCreate(animationA);
    const auto managedB = AnimationManager::getOrCreate(animationB);

    CHECK(managedA != managedB);
}

TEST_CASE("AnimationManager canonicalizes equivalent Scaling animations") {
    AnimationManagerGuard guard;

    const auto animationA = std::make_shared<Scaling>(
        Animation::BOUNCE, 2.5f, glm::vec3(2.0f, 3.0f, 4.0f)
    );
    const auto animationB = std::make_shared<Scaling>(
        Animation::BOUNCE, 2.5f, glm::vec3(2.0f, 3.0f, 4.0f)
    );

    const auto managedA = AnimationManager::getOrCreate(animationA);
    const auto managedB = AnimationManager::getOrCreate(animationB);

    CHECK(managedA == managedB);
}

TEST_CASE("AnimationManager distinguishes Scaling animations with different target scale") {
    AnimationManagerGuard guard;

    const auto animationA = std::make_shared<Scaling>(
        Animation::BOUNCE, 2.5f, glm::vec3(2.0f, 3.0f, 4.0f)
    );
    const auto animationB = std::make_shared<Scaling>(
        Animation::BOUNCE, 2.5f, glm::vec3(2.0f, 3.0f, 5.0f)
    );

    const auto managedA = AnimationManager::getOrCreate(animationA);
    const auto managedB = AnimationManager::getOrCreate(animationB);

    CHECK(managedA != managedB);
}

TEST_CASE("AnimationManager canonicalizes equivalent CurveAnimation instances by curve equality") {
    AnimationManagerGuard guard;

    const auto animationA = std::make_shared<CurveAnimation>(makeCurveA(), 2.0f, Animation::ONCE);
    const auto animationB = std::make_shared<CurveAnimation>(makeCurveA(), 2.0f, Animation::ONCE);

    const auto managedA = AnimationManager::getOrCreate(animationA);
    const auto managedB = AnimationManager::getOrCreate(animationB);

    CHECK(managedA == managedB);
}

TEST_CASE("AnimationManager distinguishes CurveAnimation instances with different curves") {
    AnimationManagerGuard guard;

    const auto animationA = std::make_shared<CurveAnimation>(makeCurveA(), 2.0f, Animation::ONCE);
    const auto animationB = std::make_shared<CurveAnimation>(makeCurveB(), 2.0f, Animation::ONCE);

    const auto managedA = AnimationManager::getOrCreate(animationA);
    const auto managedB = AnimationManager::getOrCreate(animationB);

    CHECK(managedA != managedB);
}

TEST_CASE("AnimationManager canonicalizes equivalent KeyframeAnimation instances") {
    AnimationManagerGuard guard;

    const auto animationA = std::make_shared<KeyframeAnimation>(Animation::ONCE);
    const auto animationB = std::make_shared<KeyframeAnimation>(Animation::ONCE);

    animationA->addKeyframe(1.0f, makeTransform(glm::vec3(0.0f, 0.0f, 0.0f)));
    animationA->addKeyframe(3.0f, makeTransform(glm::vec3(10.0f, 0.0f, 0.0f)));

    animationB->addKeyframe(1.0f, makeTransform(glm::vec3(0.0f, 0.0f, 0.0f)));
    animationB->addKeyframe(3.0f, makeTransform(glm::vec3(10.0f, 0.0f, 0.0f)));

    const auto managedA = AnimationManager::getOrCreate(animationA);
    const auto managedB = AnimationManager::getOrCreate(animationB);

    CHECK(managedA == managedB);
}

TEST_CASE("AnimationManager distinguishes KeyframeAnimation instances with different keyframes") {
    AnimationManagerGuard guard;

    const auto animationA = std::make_shared<KeyframeAnimation>(Animation::ONCE);
    const auto animationB = std::make_shared<KeyframeAnimation>(Animation::ONCE);

    animationA->addKeyframe(1.0f, makeTransform(glm::vec3(0.0f, 0.0f, 0.0f)));
    animationA->addKeyframe(3.0f, makeTransform(glm::vec3(10.0f, 0.0f, 0.0f)));

    animationB->addKeyframe(1.0f, makeTransform(glm::vec3(0.0f, 0.0f, 0.0f)));
    animationB->addKeyframe(3.0f, makeTransform(glm::vec3(12.0f, 0.0f, 0.0f)));

    const auto managedA = AnimationManager::getOrCreate(animationA);
    const auto managedB = AnimationManager::getOrCreate(animationB);

    CHECK(managedA != managedB);
}

TEST_CASE("AnimationManager canonicalizes equivalent Orbiting animations") {
    AnimationManagerGuard guard;

    const auto animationA = std::make_shared<Orbiting>(
        Animation::LOOP, 2.0f, 5.0f, glm::vec3(0.0f, 2.0f, 0.0f), 180.0f
    );
    const auto animationB = std::make_shared<Orbiting>(
        Animation::LOOP, 2.0f, 5.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f
    );

    const auto managedA = AnimationManager::getOrCreate(animationA);
    const auto managedB = AnimationManager::getOrCreate(animationB);

    CHECK(managedA == managedB);
}

TEST_CASE("AnimationManager distinguishes Orbiting animations with different radius") {
    AnimationManagerGuard guard;

    const auto animationA = std::make_shared<Orbiting>(
        Animation::LOOP, 2.0f, 5.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f
    );
    const auto animationB = std::make_shared<Orbiting>(
        Animation::LOOP, 2.0f, 6.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f
    );

    const auto managedA = AnimationManager::getOrCreate(animationA);
    const auto managedB = AnimationManager::getOrCreate(animationB);

    CHECK(managedA != managedB);
}

TEST_CASE("AnimationManager distinguishes Orbiting animations with different axis") {
    AnimationManagerGuard guard;

    const auto animationA = std::make_shared<Orbiting>(
        Animation::LOOP, 2.0f, 5.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f
    );
    const auto animationB = std::make_shared<Orbiting>(
        Animation::LOOP, 2.0f, 5.0f, glm::vec3(1.0f, 0.0f, 0.0f), 180.0f
    );

    const auto managedA = AnimationManager::getOrCreate(animationA);
    const auto managedB = AnimationManager::getOrCreate(animationB);

    CHECK(managedA != managedB);
}

TEST_CASE("AnimationManager distinguishes Orbiting animations with different angle") {
    AnimationManagerGuard guard;

    const auto animationA = std::make_shared<Orbiting>(
        Animation::LOOP, 2.0f, 5.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f
    );
    const auto animationB = std::make_shared<Orbiting>(
        Animation::LOOP, 2.0f, 5.0f, glm::vec3(0.0f, 1.0f, 0.0f), 90.0f
    );

    const auto managedA = AnimationManager::getOrCreate(animationA);
    const auto managedB = AnimationManager::getOrCreate(animationB);

    CHECK(managedA != managedB);
}

TEST_CASE("AnimationManager distinguishes animations of different types") {
    AnimationManagerGuard guard;

    const auto translation = std::make_shared<Translation>(
        Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 5.0f
    );
    const auto rotation = std::make_shared<Rotation>(
        Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 90.0f
    );

    const auto managedTranslation = AnimationManager::getOrCreate(translation);
    const auto managedRotation = AnimationManager::getOrCreate(rotation);

    CHECK(managedTranslation != managedRotation);
}

TEST_CASE("AnimationManager distinguishes Orbiting from Rotation with matching shared rotation properties") {
    AnimationManagerGuard guard;

    const auto orbiting = std::make_shared<Orbiting>(
        Animation::ONCE, 2.0f, 5.0f, glm::vec3(0.0f, 1.0f, 0.0f), 90.0f
    );
    const auto rotation = std::make_shared<Rotation>(
        Animation::ONCE, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 90.0f
    );

    const auto managedOrbiting = AnimationManager::getOrCreate(orbiting);
    const auto managedRotation = AnimationManager::getOrCreate(rotation);

    CHECK(managedOrbiting != managedRotation);
}

TEST_CASE("AnimationManager distinguishes animations with different mode") {
    AnimationManagerGuard guard;

    const auto animationA = std::make_shared<Translation>(
        Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 5.0f
    );
    const auto animationB = std::make_shared<Translation>(
        Animation::LOOP, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 5.0f
    );

    const auto managedA = AnimationManager::getOrCreate(animationA);
    const auto managedB = AnimationManager::getOrCreate(animationB);

    CHECK(managedA != managedB);
}

TEST_CASE("AnimationManager distinguishes animations with different duration") {
    AnimationManagerGuard guard;

    const auto animationA = std::make_shared<Translation>(
        Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 5.0f
    );
    const auto animationB = std::make_shared<Translation>(
        Animation::ONCE, 3.0f, glm::vec3(1.0f, 0.0f, 0.0f), 5.0f
    );

    const auto managedA = AnimationManager::getOrCreate(animationA);
    const auto managedB = AnimationManager::getOrCreate(animationB);

    CHECK(managedA != managedB);
}
