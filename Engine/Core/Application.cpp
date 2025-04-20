#define GLEW_STATIC
#include "../Core.h"
#include "../Geometry.h"
#include "../Graphics.h"
#include "../Scene.h"
#include "../Utilities.h"


void Application::Initialize() {
    Utilities::InitializeGLFW();
    Window::Create(1400, 900, "PixelForge");
    Utilities::InitializeGLEW();
    Camera::TurnOn(vec3(0.0f, 0.0f, 5.0f));
    TextureUnitManager::Initialize();
    Input::Enable();
}


void Application::Run() {
    Initialize();

    const auto model = make_shared<Model>("Model");
    model->AddLight(new PositionalLight("Light", vec3(0.0f)));
        model->LastAddedAs<Light>()->AddAnimation(new Orbiting(1.0f, vec3(0.0f, 1.0f, 1.0f), 360.0f, 3.0f));
        model->LastAddedAs<Light>()->SetMesh(new Sphere());
        model->LastAddedAs<Light>()->SetTransform(Scale(vec3(0.2f)));
    model->AddRenderUnit(new RenderUnit("Sphere", new Sphere()));
        model->LastAddedAs<RenderUnit>()->SetMaterial(new Gold());
        model->LastAddedAs<RenderUnit>()->AddAnimation(new Rotation(vec3(1.0f, 0.0f, 1.0f), 360.0f, 5.0f));
        model->LastAddedAs<RenderUnit>()->AddAnimation(new Orbiting(2.0f, vec3(0.0f, 1.0f, 0.0f), 360.0f, 8.0f));
    model->AddRenderUnit(new RenderUnit("Cube", new Mesh("Cube.obj")), "Sphere");
        model->LastAddedAs<RenderUnit>()->SetMaterial(new Silver());
        model->LastAddedAs<RenderUnit>()->SetTransform(Scale(vec3(0.5f)));
        model->LastAddedAs<RenderUnit>()->AddAnimation(new Orbiting(1.0f, vec3(0.5, 1.5f, -2.0f), 180.0f, 4.0, Animation::BOUNCE));
    model->AddRenderUnit(new RenderUnit("Cylinder", new Mesh("Cylinder.obj")), "Cube");
        model->LastAddedAs<RenderUnit>()->SetMaterial(new Pearl());
        model->LastAddedAs<RenderUnit>()->SetNodeScale(vec3(0.5f));
        model->LastAddedAs<RenderUnit>()->AddAnimation(new Orbiting(1.0f, vec3(0.5, -1.5f, 2.0f), 360.0f, 6.0));
    model->AddLight(new DirectionalLight("Sun", vec3(1.0f, -2.0f, 1.0f)));

    Scene scene;
    scene.AddModel(model);
    Renderer renderer; // NOLINT

    while (!Window::ShouldClose()) {
        Input::TimeStep();
        Camera::Update();
        renderer.Render(scene);
        Window::SwapBuffers();
        Input::PollEvents();
    }
    ShutDown();
}


void Application::ShutDown() {
    Window::Close();
    glfwTerminate();
}
