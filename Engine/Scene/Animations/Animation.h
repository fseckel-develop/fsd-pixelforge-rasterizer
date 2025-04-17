#pragma once
class Transform;


class Animation {
public:
    enum Mode { ONCE, LOOP, BOUNCE };
    enum Type { SCALING, ROTATION, ORBITAL, TRANSLATION };
    explicit Animation(Mode);
    void Play();
    void Pause();
    void Resume();
    void Stop();
    void Reset();
    [[nodiscard]] bool IsPlaying() const;
    [[nodiscard]] bool IsPaused() const;
    virtual Transform GetOffset(float) = 0;
    virtual ~Animation() = default;

protected:
    Mode mode;
    float duration = 1.0f;
    float elapsedTime = 0.0f;
    bool isPlaying = true;
    bool isPaused = false;
    // TODO: EasingFunction ...

    [[nodiscard]] float GetProgress() const;
};
