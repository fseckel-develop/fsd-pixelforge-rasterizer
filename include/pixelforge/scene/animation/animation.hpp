#pragma once

namespace pixelforge::scene::transform { class Transform; }

namespace pixelforge::scene::animation {

    /// Base class for time-based transforming animations.
    class Animation {
    public:
        /// Enumeration for playback status of an animation.
        enum PlaybackState { STOPPED, PLAYING, PAUSED };

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
        void setDuration(float duration);

        /// Sets the animation playback mode.
        /// @param mode The new playback mode (ONCE, LOOP, BOUNCE).
        void setMode(Mode mode);

        /// Starts the stopped animation from beginning, or resumes when paused.
        void play();

        /// Pauses the animation, preserving its current state.
        void pause();

        /// Resumes the animation from its paused state.
        void resume();

        /// Stops the animation and resets progress.
        void stop();

        /// Resets the animation time to the beginning without stopping.
        void reset();

        /// Updates the animation progress based on time.
        /// @param timeDelta Time in seconds since last update.
        void update(float timeDelta);

        /// Returns the total duration of the animation.
        /// @return The duration in seconds.
        [[nodiscard]] float getDuration() const;

        /// Returns the current animation playback mode.
        /// @return The playback mode.
        [[nodiscard]] Mode getMode() const;

        /// Checks whether the animation is actively playing.
        /// @return True if playing, false otherwise.
        [[nodiscard]] bool isPlaying() const;

        /// Checks whether the animation is currently paused.
        /// @return True if paused.
        [[nodiscard]] bool isPaused() const;

        /// Computes the animated transform at the current time. To be overridden.
        /// @return Transform object representing the offset.
        [[nodiscard]] virtual transform::Transform getOffset() const = 0;

        /// Virtual destructor.
        virtual ~Animation() = default;

    protected:
        Mode mode_;                 ///< Playback mode (ONCE, LOOP, BOUNCE).
        float duration_;            ///< Total duration of the animation in seconds.
        float elapsedTime_ = 0.0f;  ///< Time accumulated since animation started.
        PlaybackState playbackState_ = PLAYING; ///< The current playback state.

        /// Computes normalized progress (0.0 to 1.0) based on elapsed time and mode.
        /// @return The current progress of the animation.
        [[nodiscard]] float getProgress() const;
    };

} // pixelforge::scene::animation
