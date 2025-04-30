#pragma once
class Transform;


class Animation {
public:
    enum Mode { ONCE, LOOP, BOUNCE };
    enum Type { SCALING, ROTATION, ORBITAL, TRANSLATION };
    explicit Animation(Mode);
    explicit Animation(Mode, float);
    void SetDuration(float);
    void SetMode(Mode);
    void Play();
    void Pause();
    void Resume();
    void Stop();
    void Reset();
    void Update(float deltaTime);
    [[nodiscard]] float GetDuration() const;
    [[nodiscard]] Mode GetMode() const;
    [[nodiscard]] bool IsPlaying() const;
    [[nodiscard]] bool IsPaused() const;
    [[nodiscard]] virtual Transform GetOffset() = 0;
    virtual ~Animation() = default;

protected:
    Mode mode;
    float duration;
    float elapsedTime = 0.0f;
    bool isPlaying = true;
    bool isPaused = false;
    // TODO: EasingFunction ...
    [[nodiscard]] float GetProgress() const;
};
