#pragma once


/// Manages the lifecycle of the PixelForge application.
class Application {
public:
    /// Initializes all systems and enters the main render loop until the window is closed.
    static void Run();

private:
    /// Sets up windowing, rendering, input, camera, and texture management.
    static void Initialize();

    /// Closes the window and terminates any backend systems.
    static void ShutDown();
};
