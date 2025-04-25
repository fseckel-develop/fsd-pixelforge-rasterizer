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
    Input::Enable();
    TextureUnitManager::Initialize();
    Renderer::Initialize();
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
        model->LastAddedAs<RenderUnit>()->AddAnimation(new Orbiting(2.0f, vec3(0.5, 1.5f, -2.0f), 360.0f, 4.0));
    model->AddRenderUnit(new RenderUnit("Cylinder", new Mesh("Cylinder.obj")), "Cube");
        model->LastAddedAs<RenderUnit>()->SetMaterial(new Bronze());
        model->LastAddedAs<RenderUnit>()->SetNodeScale(vec3(0.5f));
        model->LastAddedAs<RenderUnit>()->AddAnimation(new Orbiting(1.5f, vec3(0.5, -1.5f, 2.0f), 360.0f, 6.0));
    model->AddLight(new AmbientLight("Ambient", vec3(1.0f), 0.5f));

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
