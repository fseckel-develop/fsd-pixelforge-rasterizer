#define GLEW_STATIC
#include "../Core.h"
#include "../Geometry.h"
#include "../Managers.h"
#include "../Scene.h"
#include "../Utilities.h"


void Application::Initialize() {
    Utilities::InitializeGLFW();
    Window::Create(1400, 900, "PixelForge");
    Utilities::InitializeGLEW();
    Camera::TurnOn(vec3(0.0f, 0.0f, 5.0f));
    Input::Enable();
    Renderer::Initialize();
    TextureManager::Initialize();
}


void Application::Run() {
    Initialize();

    const auto model = Model_("Model")
        .With(LightUnit_("Ambient")
            .withLight(AmbientLight_().withIntensity(0.5f)))
        .With(LightUnit_("Light")
            .withLight(PositionalLight_())
            .withAnimation(make_shared<Orbiting>(1.0f, vec3(0.0f, 1.0f, 1.0f), 360.0f, 3.0f))
            .withNodeScale(vec3(0.15f)))
        .With(RenderUnit_("Sphere")
            .withMesh(make_shared<Sphere>())
            .withMaterial(make_shared<Gold>())
            .withAnimation(make_shared<Rotation>(vec3(1.0f, 0.0f, 1.0f), 360.0f, 5.0f))
            .withAnimation(make_shared<Orbiting>(2.0f, vec3(0.0f, 1.0f, 0.0f), 360.0f, 8.0f)))
        .With(RenderUnit_("Cube", "Sphere")
            .withMesh(make_shared<Mesh>("Cube.obj"))
            .withMaterial(make_shared<Silver>())
            .withTransform(Scale(vec3(0.5f)))
            .withAnimation(make_shared<Orbiting>(2.0f, vec3(0.5, 1.5f, -2.0f), 360.0f, 4.0)))
        .With(RenderUnit_("Cylinder", "Cube")
            .withMesh(make_shared<Mesh>("Cylinder.obj"))
            .withMaterial(make_shared<Bronze>())
            .withNodeScale(vec3(0.5f))
            .withAnimation(make_shared<Orbiting>(1.5f, vec3(0.5, -1.5f, 2.0f), 360.0f, 6.0)))
        .Build();

    const auto scene = make_shared<Scene>("Scene");
    scene->AddModel(model);

    while (!Window::ShouldClose()) {
        Input::TimeStep();
        Camera::Update();
        Renderer::Render(scene);
        Window::SwapBuffers();
        Input::PollEvents();
    }
    ShutDown();
}


void Application::ShutDown() {
    Window::Close();
    glfwTerminate();
}
