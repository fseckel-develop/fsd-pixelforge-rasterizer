#include <doctest.h>
#include <pixelforge/scene/animation/animation.hpp>
#include <pixelforge/scene/transform/transform.hpp>

using pixelforge::scene::animation::Animation;
using pixelforge::scene::transform::Transform;


namespace {

    class TestAnimation final : public Animation {
    public:
        explicit TestAnimation(const Mode mode) :
            Animation(mode) {
        }

        TestAnimation(const Mode mode, const float duration) :
            Animation(mode, duration) {
        }

        [[nodiscard]] Transform getOffset() const override {
            return {};
        }

        [[nodiscard]] float progress() const {
            return getProgress();
        }
    };

} // namespace


TEST_CASE("Animation constructor sets default duration to one second") {
    const TestAnimation animation(Animation::ONCE);

    CHECK(animation.getMode() == Animation::ONCE);
    CHECK(animation.getDuration() == doctest::Approx(1.0f));
    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());
    CHECK(animation.progress() == doctest::Approx(0.0f));
}

TEST_CASE("Animation constructor clamps duration to minimum") {
    const TestAnimation animation(Animation::LOOP, 0.0f);

    CHECK(animation.getMode() == Animation::LOOP);
    CHECK(animation.getDuration() == doctest::Approx(0.01f));
}

TEST_CASE("Animation setDuration clamps duration to minimum") {
    TestAnimation animation(Animation::ONCE, 2.0f);

    animation.setDuration(-5.0f);

    CHECK(animation.getDuration() == doctest::Approx(0.01f));
}

TEST_CASE("Animation setMode updates playback mode") {
    TestAnimation animation(Animation::ONCE);

    animation.setMode(Animation::BOUNCE);

    CHECK(animation.getMode() == Animation::BOUNCE);
}

TEST_CASE("Animation pause sets paused state") {
    TestAnimation animation(Animation::ONCE);

    animation.pause();

    CHECK(animation.isPaused());
    CHECK_FALSE(animation.isPlaying());
}

TEST_CASE("Animation resume clears paused state") {
    TestAnimation animation(Animation::ONCE);

    animation.pause();
    REQUIRE(animation.isPaused());

    animation.resume();

    CHECK_FALSE(animation.isPaused());
    CHECK(animation.isPlaying());
}

TEST_CASE("Animation stop resets elapsed time and clears play and pause state") {
    TestAnimation animation(Animation::ONCE, 2.0f);

    animation.update(0.5f);
    CHECK(animation.progress() == doctest::Approx(0.25f));

    animation.pause();
    CHECK(animation.isPaused());

    animation.stop();

    CHECK_FALSE(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());
    CHECK(animation.progress() == doctest::Approx(0.0f));
}

TEST_CASE("Animation reset resets elapsed time without changing stopped state") {
    TestAnimation animation(Animation::ONCE, 2.0f);

    animation.update(1.0f);
    CHECK(animation.progress() == doctest::Approx(0.5f));

    animation.stop();
    CHECK_FALSE(animation.isPlaying());

    animation.reset();

    CHECK_FALSE(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());
    CHECK(animation.progress() == doctest::Approx(0.0f));
}

TEST_CASE("Animation reset resets elapsed time while preserving play state") {
    TestAnimation animation(Animation::ONCE, 2.0f);

    animation.update(1.0f);
    CHECK(animation.progress() == doctest::Approx(0.5f));
    CHECK(animation.isPlaying());

    animation.reset();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());
    CHECK(animation.progress() == doctest::Approx(0.0f));
}

TEST_CASE("Animation update advances elapsed time while playing") {
    TestAnimation animation(Animation::ONCE, 2.0f);

    animation.update(0.5f);

    CHECK(animation.progress() == doctest::Approx(0.25f));
}

TEST_CASE("Animation update does not advance while paused") {
    TestAnimation animation(Animation::ONCE, 2.0f);

    animation.pause();
    REQUIRE(animation.isPaused());

    animation.update(0.5f);

    CHECK(animation.progress() == doctest::Approx(0.0f));
    CHECK(animation.isPaused());
    CHECK_FALSE(animation.isPlaying());
}

TEST_CASE("Animation update does not advance after stop") {
    TestAnimation animation(Animation::ONCE, 2.0f);

    animation.stop();
    animation.update(1.0f);

    CHECK(animation.progress() == doctest::Approx(0.0f));
}

TEST_CASE("Animation ONCE progress clamps at one") {
    TestAnimation animation(Animation::ONCE, 2.0f);

    animation.update(5.0f);

    CHECK(animation.progress() == doctest::Approx(1.0f));
    CHECK_FALSE(animation.isPlaying());
}

TEST_CASE("Animation LOOP progress wraps around") {
    TestAnimation animation(Animation::LOOP, 2.0f);

    animation.update(0.5f);
    CHECK(animation.progress() == doctest::Approx(0.25f));

    animation.update(2.0f);
    CHECK(animation.progress() == doctest::Approx(0.25f));

    CHECK(animation.isPlaying());
}

TEST_CASE("Animation BOUNCE progress reflects back after one") {
    TestAnimation animation(Animation::BOUNCE, 2.0f);

    animation.update(0.5f);
    CHECK(animation.progress() == doctest::Approx(0.25f));

    animation.update(1.5f);
    CHECK(animation.progress() == doctest::Approx(1.0f));

    animation.update(0.5f);
    CHECK(animation.progress() == doctest::Approx(0.75f));

    animation.update(1.5f);
    CHECK(animation.progress() == doctest::Approx(0.0f));

    CHECK(animation.isPlaying());
}

TEST_CASE("Animation play resumes paused animation without resetting time") {
    TestAnimation animation(Animation::ONCE, 2.0f);

    animation.update(0.5f);
    animation.pause();
    REQUIRE(animation.isPaused());
    CHECK(animation.progress() == doctest::Approx(0.25f));

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());
    CHECK(animation.progress() == doctest::Approx(0.25f));
}

TEST_CASE("Animation play restarts stopped animation from beginning") {
    TestAnimation animation(Animation::ONCE, 2.0f);

    animation.update(0.5f);
    CHECK(animation.progress() == doctest::Approx(0.25f));

    animation.stop();
    CHECK(animation.progress() == doctest::Approx(0.0f));

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());
    CHECK(animation.progress() == doctest::Approx(0.0f));
}

TEST_CASE("Animation play restarts finished ONCE animation from beginning") {
    TestAnimation animation(Animation::ONCE, 1.0f);

    animation.update(2.0f);
    CHECK(animation.progress() == doctest::Approx(1.0f));
    CHECK_FALSE(animation.isPlaying());

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());
    CHECK(animation.progress() == doctest::Approx(0.0f));
}

TEST_CASE("Animation resume has no effect when stopped") {
    TestAnimation animation(Animation::ONCE);

    animation.stop();
    REQUIRE_FALSE(animation.isPlaying());

    animation.resume();

    CHECK_FALSE(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());
    CHECK(animation.progress() == doctest::Approx(0.0f));
}

TEST_CASE("Animation pause has no effect when already stopped") {
    TestAnimation animation(Animation::ONCE);

    animation.stop();
    REQUIRE_FALSE(animation.isPlaying());

    animation.pause();

    CHECK_FALSE(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());
}
