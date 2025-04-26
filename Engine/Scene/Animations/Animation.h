#pragma once
class Transform;


class Animation {
public:
    enum Mode { ONCE, LOOP, BOUNCE };
    enum Type { SCALING, ROTATION, ORBITAL, TRANSLATION };
    explicit Animation(float, Mode);
    void SetDuration(float);
    void Play();
    void Pause();
    void Resume();
    void Stop();
    void Reset();
    void Update(float deltaTime);
    [[nodiscard]] Mode GetMode() const;
    [[nodiscard]] float GetDuration() const;
    [[nodiscard]] bool IsPlaying() const;
    [[nodiscard]] bool IsPaused() const;
    [[nodiscard]] virtual Transform GetOffset() = 0;
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
