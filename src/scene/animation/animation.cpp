#include <pixelforge/scene/animation/animation.hpp>
#include <GLM/glm.hpp>


namespace pixelforge::scene::animation {

    using glm::clamp;


    Animation::Animation(const Mode mode):
        mode_(mode),
        duration_(1.0f) {
    }


    Animation::Animation(const Mode mode, const float duration):
        mode_(mode),
        duration_(duration <= 0.01f ? 0.01f : duration) {
    }


    void Animation::setDuration(const float duration) {
        this->duration_ = duration <= 0.01f ? 0.01f : duration;
    }


    void Animation::setMode(const Mode mode) {
        this->mode_ = mode;
    }


    void Animation::play() {
        if (!isPlaying_ || isPaused_) {
            isPlaying_ = true;
            isPaused_ = false;
        }
    }


    void Animation::pause() {
        if (isPlaying_) {
            isPaused_ = true;
        }
    }


    void Animation::resume() {
        if (isPaused_) {
            isPaused_ = false;
        }
    }


    void Animation::stop() {
        isPlaying_ = false;
        isPaused_ = false;
        elapsedTime_ = 0.0f;
    }


    void Animation::reset() {
        elapsedTime_ = 0.0f;
    }


    void Animation::update(const float timeDelta) {
        if (isPlaying_) {
            elapsedTime_ += timeDelta;
        }
    }


    Animation::Mode Animation::getMode() const {
        return mode_;
    }


    float Animation::getDuration() const {
        return duration_;
    }


    bool Animation::isPlaying() const {
        if (!isPlaying_ || isPaused_) return false;
        if (mode_ == LOOP || mode_ == BOUNCE) return true;
        return elapsedTime_ < duration_;
    }


    bool Animation::isPaused() const {
        return isPaused_;
    }


    float Animation::getProgress() const {
        if (mode_ == ONCE) return clamp(elapsedTime_ / duration_, 0.0f, 1.0f);
        if (mode_ == LOOP) return fmod(elapsedTime_ / duration_, 1.0f);
        if (mode_ == BOUNCE) {
            const float progress = fmod(elapsedTime_ / duration_, 2.0f);
            if (progress > 1.0f) return 2.0f - progress;
            return progress;
        }
        return 0.0f;
    }

} // namespace pixelforge::scene::animation
