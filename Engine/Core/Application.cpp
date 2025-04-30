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

    const auto scene = Scene_("Scene")
    .With(Model_("Model")
        .With(LightUnit_("Ambient")
            .withLight(AmbientLight_().withIntensity(0.5f)))
        .With(LightUnit_("Light")
            .withLight(PositionalLight_())
            .withTransform(Translation(0.0f, -2.0f, 0.0f))
            .withAnimation(Translating_(Animation::BOUNCE)
                .withDuration(1.0f).withTotalDistance(4.0f).withDirection({0.0f, 1.0f, 0.0f}))
            .withNodeScale(0.15f))
        .With(RenderUnit_("Sphere")
            .withMesh(Sphere())
            .withMaterial(Gold())
            .withAnimation(Rotating_(Animation::LOOP)
                .withDuration(5.0f).withRotationAxis({1.0f, 0.0f, 1.0f}))
            .withAnimation(Orbiting_(Animation::LOOP)
                .withDuration(8.0f).withRadius(2.0f).withRotationAxis({0.0f, 1.0f, 0.0f})))
        .With(RenderUnit_("Cube", "Sphere")
            .withMesh("Cube.obj")
            .withMaterial(Silver())
            .withTransform(Scale(0.5f))
            .withAnimation(Orbiting_(Animation::LOOP)
                .withDuration(4.0f).withRadius(2.0f).withRotationAxis({0.5, 1.5f, -2.0})))
        .With(RenderUnit_("Cylinder", "Cube")
            .withMesh("Cylinder.obj")
            .withMaterial(Bronze())
            .withNodeScale(0.5f)
            .withAnimation(Orbiting_(Animation::LOOP)
                .withDuration(6.0f).withRadius(1.5f).withRotationAxis({0.5, -1.5f, 2.0}))))
    .Build();

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
