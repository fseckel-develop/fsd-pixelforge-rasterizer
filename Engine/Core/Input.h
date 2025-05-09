#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>


/// Represents a 2D screen-space coordinate.
typedef struct Position {
    double x; ///< Horizontal screen position.
    double y; ///< Vertical screen position.
} Position;


/// Wraps GLFW input callbacks and polling, tracks current
/// input states, and provides delta time per frame.
class Input {
public:
    /// Enables input processing and sets up GLFW callbacks.
    static void Enable();

    /// Returns the current mouse position.
    static Position GetMousePosition();

    /// Returns the mouse movement delta since last frame.
    static Position GetMouseDelta();

    /// Returns whether a mouse button is currently pressed or not.
    /// @param button GLFW mouse button code.
    static bool IsMouseButtonPressed(int button);

    /// Returns whether a keyboard key is currently pressed or not.
    /// @param key GLFW key code.
    static bool IsKeyPressed(int key);

    /// Returns the time delta between the current and last frame.
    static float GetDeltaTime();

    /// Updates time-related variables for this frame.
    static void TimeStep();

    /// Polls GLFW events and updates internal state.
    static void PollEvents();

private:
    static bool mouseHasMoved;      ///< True if the mouse moved this frame.
    static Position mousePosition;  ///< Current mouse position in screen space.
    static Position mouseDelta;     ///< Mouse movement delta since last frame.
    static std::unordered_map<int, bool> mouseButtonStates; ///< Map of current mouse button press states.
    static std::unordered_map<int, bool> keyStates;         ///< Map of current key press states.
    static float currentTime;   ///< Current time in seconds since start.
    static float lastTime;      ///< Time at the previous frame.
    static float deltaTime;     ///< Time elapsed between the last two frames.

    /// GLFW callback: updates internal mouse position and its delta.
    /// @param window The GLFW window receiving the event.
    /// @param newX New X position of the mouse.
    /// @param newY New Y position of the mouse.
    static void MousePositionCallback(GLFWwindow* window, double newX, double newY);

    /// GLFW callback: updates mouse button states.
    /// @param window The GLFW window receiving the event.
    /// @param button The mouse button that changed state.
    /// @param action The action (GLFW_PRESS or GLFW_RELEASE).
    /// @param mods Modifier key bitfield (e.g. shift, ctrl).
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    /// GLFW callback: updates key states; ESC closes window.
    /// @param window The GLFW window receiving the event.
    /// @param key The keyboard key that changed state.
    /// @param scancode Platform-specific scancode of the key.
    /// @param action The action (GLFW_PRESS, GLFW_RELEASE, or GLFW_REPEAT).
    /// @param mods Modifier key bitfield (e.g. shift, ctrl).
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
