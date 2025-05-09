#pragma once
class Transform;


/// Base class for time-based transforming animations.
class Animation {
public:
    /// Enumeration for how the animation progresses.
    enum Mode { ONCE, LOOP, BOUNCE };

    /// Enumeration for animation types, used for ordering.
    enum Type { SCALING, ROTATION, ORBITAL, TRANSLATION };
    // TODO: Ordering Animations attached to an animation node.

    /// Constructor with playback mode and default duration of 1.0 second.
    /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
    explicit Animation(Mode mode);

    /// Constructor with animation mode and custom duration.
    /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
    /// @param duration The total duration in seconds.
    explicit Animation(Mode mode, float duration);

    /// Sets the total duration of the animation.
    /// @param duration The new duration in seconds (Minimum is 0.01).
    void SetDuration(float duration);

    /// Sets the animation playback mode.
    /// @param mode The new playback mode (ONCE, LOOP, BOUNCE).
    void SetMode(Mode mode);

    /// Starts or resumes the animation from beginning or pause.
    void Play();

    /// Pauses the animation, preserving its current state.
    void Pause();

    /// Resumes the animation from its paused state.
    void Resume();

    /// Stops the animation and resets progress.
    void Stop();

    /// Resets the animation time to the beginning without stopping.
    void Reset();

    /// Updates the animation progress based on time.
    /// @param deltaTime Time in seconds since last update.
    void Update(float deltaTime);

    /// Returns the total duration of the animation.
    /// @return The duration in seconds.
    [[nodiscard]] float GetDuration() const;

    /// Returns the current animation playback mode.
    /// @return The playback mode.
    [[nodiscard]] Mode GetMode() const;

    /// Checks whether the animation is actively playing.
    /// @return True if playing, false otherwise.
    [[nodiscard]] bool IsPlaying() const;

    /// Checks whether the animation is currently paused.
    /// @return True if paused.
    [[nodiscard]] bool IsPaused() const;

    /// Computes the animated transform at the current time. To be overridden.
    /// @return Transform object representing the offset.
    [[nodiscard]] virtual Transform GetOffset() = 0;

    /// Virtual destructor.
    virtual ~Animation() = default;

protected:
    Mode mode; ///< Playback mode (ONCE, LOOP, BOUNCE).
    float duration; ///< Total duration of the animation in seconds.
    float elapsedTime = 0.0f; ///< Time accumulated since animation started.
    bool isPlaying = true; ///< Indicates whether the animation is currently playing.
    bool isPaused = false; ///< Indicates whether the animation is paused.

    /// Computes normalized progress (0.0 to 1.0) based on elapsed time and mode.
    /// @return The current progress of the animation.
    [[nodiscard]] float GetProgress() const;
};
