#define GLEW_STATIC
#include "../Core.h"
#include "../Utilities.h"


void Application::Initialize() {
    Utilities::InitializeGLFW();
    Window::Create(1400, 900, "PixelForge");
    Utilities::InitializeGLEW();
    Camera::TurnOn(vec3(0.0f, 0.0f, 3.0f));
    Input::Enable();
}


void Application::Run() {
    Initialize();

    while (!Window::ShouldClose()) {
        Input::TimeStep();
        Camera::Update();

        Window::SwapBuffers();
        Input::PollEvents();
    }

    ShutDown();
}


void Application::ShutDown() {
    Window::Close();
    glfwTerminate();
}
