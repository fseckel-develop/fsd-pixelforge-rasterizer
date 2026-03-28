#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>


namespace pixelforge::core {

    /// Represents a 2D screen-space coordinate.
    struct Position {
        double x; ///< Horizontal screen position.
        double y; ///< Vertical screen position.
    };


    /// Wraps GLFW input callbacks and polling, tracks current
    /// input states, and provides delta time per frame.
    class Input {
    public:
        /// Enables input processing and sets up GLFW callbacks.
        static void enable();

        /// Returns the current mouse position.
        static Position getMousePosition();

        /// Returns the mouse movement delta since last frame.
        static Position getMouseDelta();

        /// Returns whether a mouse button is currently pressed or not.
        /// @param button GLFW mouse button code.
        static bool isMouseButtonPressed(int button);

        /// Returns whether a keyboard key is currently pressed or not.
        /// @param key GLFW key code.
        static bool isKeyPressed(int key);

        /// Returns the time delta between the current and last frame.
        static float getTimeDelta();

        /// Updates time-related variables for this frame.
        static void timeStep();

        /// Polls GLFW events and updates internal state.
        static void pollEvents();

    private:
        static bool mouseHasMoved_;      ///< True if the mouse moved this frame.
        static Position mousePosition_;  ///< Current mouse position in screen space.
        static Position mouseDelta_;     ///< Mouse movement delta since last frame.
        static std::unordered_map<int, bool> mouseButtonStates_; ///< Map of current mouse button press states.
        static std::unordered_map<int, bool> keyStates_;         ///< Map of current key press states.
        static float currentTime_;       ///< Current time in seconds since start.
        static float lastTime_;          ///< Time at the previous frame.
        static float timeDelta_;         ///< Time elapsed between the last two frames.
        static bool timeIsFrozen_;       ///< Whether time progressing is frozen.

        /// GLFW callback: updates internal mouse position and its delta.
        /// @param window The GLFW window receiving the event.
        /// @param newX New X position of the mouse.
        /// @param newY New Y position of the mouse.
        static void mousePositionCallback(GLFWwindow* window, double newX, double newY);

        /// GLFW callback: updates mouse button states.
        /// @param window The GLFW window receiving the event.
        /// @param button The mouse button that changed state.
        /// @param action The action (GLFW_PRESS or GLFW_RELEASE).
        /// @param mods Modifier key bitfield (e.g. shift, ctrl).
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

        /// GLFW callback: updates key states; ESC closes window.
        /// @param window The GLFW window receiving the event.
        /// @param key The keyboard key that changed state.
        /// @param scancode Platform-specific scancode of the key.
        /// @param action The action (GLFW_PRESS, GLFW_RELEASE, or GLFW_REPEAT).
        /// @param mods Modifier key bitfield (e.g. shift, ctrl).
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    };

} // namespace pixelforge::core
