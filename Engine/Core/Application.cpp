#define GLEW_STATIC
#include "../Core.h"
#include "../Geometry.h"
#include "../Graphics.h"
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

    const auto model = shared<Model>("Model");
    model->AddLight("Light", shared<PositionalLight>(vec3(0.0f)));
        model->LastAddedAs<LightNode>()->AddAnimation(shared<Orbiting>(1.0f, vec3(0.0f, 1.0f, 1.0f), 360.0f, 3.0f));
        model->LastAddedAs<LightNode>()->SetMesh(shared<Sphere>());
        model->LastAddedAs<LightNode>()->SetTransform(Scale(vec3(0.1f)));
    model->AddRenderUnit("Sphere", shared<Sphere>());
        model->LastAddedAs<RenderUnit>()->SetMaterial(shared<Gold>());
        model->LastAddedAs<RenderUnit>()->AddAnimation(shared<Rotation>(vec3(1.0f, 0.0f, 1.0f), 360.0f, 5.0f));
        model->LastAddedAs<RenderUnit>()->AddAnimation(shared<Orbiting>(2.0f, vec3(0.0f, 1.0f, 0.0f), 360.0f, 8.0f));
    model->AddRenderUnit("Cube", shared<Mesh>("Cube.obj"), "Sphere");
        model->LastAddedAs<RenderUnit>()->SetMaterial(shared<Silver>());
        model->LastAddedAs<RenderUnit>()->SetTransform(Scale(vec3(0.5f)));
        model->LastAddedAs<RenderUnit>()->AddAnimation(shared<Orbiting>(2.0f, vec3(0.5, 1.5f, -2.0f), 360.0f, 4.0));
    model->AddRenderUnit("Cylinder", shared<Mesh>("Cylinder.obj"), "Cube");
        model->LastAddedAs<RenderUnit>()->SetMaterial(shared<Bronze>());
        model->LastAddedAs<RenderUnit>()->SetNodeScale(vec3(0.5f));
        model->LastAddedAs<RenderUnit>()->AddAnimation(shared<Orbiting>(1.5f, vec3(0.5, -1.5f, 2.0f), 360.0f, 6.0));
    model->AddLight("Ambient", shared<AmbientLight>(vec3(1.0f), 0.5f));

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
