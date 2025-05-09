#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>


/// Manages the main application window using GLFW, handling creation,
/// sizing, buffer dimensions, viewport setup, and destruction.
class Window {
public:
    /// Creates the GLFW window with given dimensions and title.
    /// @param width Width of the window in screen coordinates.
    /// @param height Height of the window in screen coordinates.
    /// @param title Title of the window.
    static void Create(GLint width, GLint height, const char* title);

    /// Swaps the front and back buffers.
    static void SwapBuffers();

    /// Sets the OpenGL viewport to match the framebuffer size.
    static void SetViewport();

    /// Checks if the window should close (e.g., user pressed close button).
    /// @return True if window is flagged to close, false otherwise.
    static bool ShouldClose();

    /// Gets the raw GLFWwindow pointer.
    /// @return Pointer to the GLFWwindow.
    static GLFWwindow* GetWindow();

    /// Gets the framebuffer width in pixels.
    /// @return Buffer width.
    static GLint GetBufferWidth();

    /// Gets the framebuffer height in pixels.
    /// @return Buffer height.
    static GLint GetBufferHeight();

    /// Calculates and returns the current aspect ratio.
    /// @return Aspect ratio as bufferWidth / bufferHeight.
    static float GetAspectRatio();

    /// Destroys and cleans up the window.
    static void Close();

private:
    static GLFWwindow* window; ///< Pointer to the GLFW window.
    static const char* windowTitle; ///< Window title string.
    static GLint windowWidth, windowHeight; ///< Window size in screen coordinates.
    static GLint bufferWidth, bufferHeight; ///< Framebuffer size in pixels (may differ from window size on high-DPI displays).

    /// GLFW callback for when the window size is changed.
    /// @param window The GLFW window.
    /// @param newWidth New window width in screen coordinates.
    /// @param newHeight New window height in screen coordinates.
    static void WindowSizeCallback(GLFWwindow* window, int newWidth, int newHeight);

    /// GLFW callback for when the framebuffer size changes.
    /// @param window The GLFW window.
    /// @param newBufferWidth New framebuffer width in pixels.
    /// @param newBufferHeight New framebuffer height in pixels.
    static void FramebufferSizeCallback(GLFWwindow* window, int newBufferWidth, int newBufferHeight);
};