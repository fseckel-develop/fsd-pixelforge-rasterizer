#pragma once


namespace pixelforge::core::context {

    /// Initializes the GLFW library and sets OpenGL context version and profile.
    void initializeGLFW();

    /// Initializes the GLEW library.
    void initializeGLEW();

} // namespace pixelforge::core::context
