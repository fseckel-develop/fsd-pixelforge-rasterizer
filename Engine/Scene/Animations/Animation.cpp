#include "Animation.h"
#include <GLM/glm.hpp>
using namespace glm;


Animation::Animation(const Mode mode):
    mode(mode),
    duration(1.0f) {
}


Animation::Animation(const Mode mode, const float duration):
    mode(mode),
    duration(duration <= 0.01f ? 0.01f : duration) {
}


void Animation::SetDuration(const float duration) {
    this->duration = duration <= 0.01f ? 0.01f : duration;
}


void Animation::SetMode(const Mode mode) {
    this->mode = mode;
}


void Animation::Play() {
    if (!isPlaying || isPaused) {
        isPlaying = true;
        isPaused = false;
    }
}


void Animation::Pause() {
    if (isPlaying) {
        isPaused = true;
    }
}


void Animation::Resume() {
    if (isPaused) {
        isPaused = false;
    }
}


void Animation::Stop() {
    isPlaying = false;
    isPaused = false;
    elapsedTime = 0.0f;
}


void Animation::Reset() {
    elapsedTime = 0.0f;
}


void Animation::Update(const float deltaTime) {
    if (isPlaying) {
        elapsedTime += deltaTime;
    }
}


Animation::Mode Animation::GetMode() const {
    return mode;
}


float Animation::GetDuration() const {
    return duration;
}


bool Animation::IsPlaying() const {
    if (!isPlaying || isPaused) return false;
    if (mode == LOOP || mode == BOUNCE) return true;
    return elapsedTime < duration;
}


bool Animation::IsPaused() const {
    return isPaused;
}


float Animation::GetProgress() const {
    if (mode == ONCE) return clamp(elapsedTime / duration, 0.0f, 1.0f);
    if (mode == LOOP) return fmod(elapsedTime / duration, 1.0f);
    if (mode == BOUNCE) {
        const float progress = fmod(elapsedTime / duration, 2.0f);
        if (progress > 1.0f) return 2.0f - progress;
        return progress;
    }
    return 0.0f;
}
