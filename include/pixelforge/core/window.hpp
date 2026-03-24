#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>


namespace pixelforge::core {

    /// Manages the main application window using GLFW, handling creation,
    /// sizing, buffer dimensions, viewport setup, and destruction.
    class Window {
    public:
        /// Creates the GLFW window with given dimensions and title.
        /// @param width Width of the window in screen coordinates.
        /// @param height Height of the window in screen coordinates.
        /// @param title Title of the window.
        static void create(GLint width, GLint height, const char* title);

        /// Swaps the front and back buffers.
        static void swapBuffers();

        /// Sets the OpenGL viewport to match the framebuffer size.
        static void setViewport();

        /// Checks if the window should close (e.g., user pressed close button).
        /// @return True if window is flagged to close, false otherwise.
        static bool shouldClose();

        /// Gets the raw GLFWwindow pointer.
        /// @return Pointer to the GLFWwindow.
        static GLFWwindow* getWindow();

        /// Gets the framebuffer width in pixels.
        /// @return Buffer width.
        static GLint getBufferWidth();

        /// Gets the framebuffer height in pixels.
        /// @return Buffer height.
        static GLint getBufferHeight();

        /// Calculates and returns the current aspect ratio.
        /// @return Aspect ratio as bufferWidth / bufferHeight.
        static float getAspectRatio();

        /// Destroys and cleans up the window.
        static void close();

    private:
        static GLFWwindow* window_; ///< Pointer to the GLFW window.
        static const char* windowTitle_; ///< Window title string.
        static GLint windowWidth_, windowHeight_; ///< Window size in screen coordinates.
        static GLint bufferWidth_, bufferHeight_; ///< Framebuffer size in pixels (may differ from window size on high-DPI displays).

        /// GLFW callback for when the window size is changed.
        /// @param window The GLFW window.
        /// @param newWidth New window width in screen coordinates.
        /// @param newHeight New window height in screen coordinates.
        static void windowSizeCallback(GLFWwindow* window, int newWidth, int newHeight);

        /// GLFW callback for when the framebuffer size changes.
        /// @param window The GLFW window.
        /// @param newBufferWidth New framebuffer width in pixels.
        /// @param newBufferHeight New framebuffer height in pixels.
        static void framebufferSizeCallback(GLFWwindow* window, int newBufferWidth, int newBufferHeight);
    };

} // namespace pixelforge::core
