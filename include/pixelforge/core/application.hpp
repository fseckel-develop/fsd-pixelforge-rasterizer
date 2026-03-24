#pragma once


namespace pixelforge::core {

    /// Manages the lifecycle of the PixelForge application.
    class Application {
    public:
        /// Initializes all systems and enters the main render loop until the window is closed.
        static void run();

    private:
        /// Sets up windowing, rendering, input, camera, and texture management.
        static void initialize();

        /// Closes the window and terminates any backend systems.
        static void shutDown();
    };

} // namespace pixelforge::core
