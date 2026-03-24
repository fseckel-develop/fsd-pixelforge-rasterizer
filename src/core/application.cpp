#define GLEW_STATIC
#include <pixelforge/core.hpp>
#include <pixelforge/geometry.hpp>
#include <pixelforge/managers.hpp>
#include <pixelforge/scene.hpp>
#include <pixelforge/utilities.hpp>


namespace pixelforge::core {

    using namespace geometry;
    using namespace graphics;
    using namespace scene::nodes;
    using namespace scene::lighting;


    void Application::initialize() {
        Utilities::initializeGLFW();
        Window::create(1400, 900, "PixelForge");
        Utilities::initializeGLEW();
        Camera::turnOn({0.0f, 0.0f, 5.0f});
        Input::enable();
        Renderer::initialize();
        management::TextureManager::initialize();
    }


    void Application::run() {
        initialize();

        const auto scene = Scene_("Scene")
        .with(Model_("Model")
            .with(LightUnit_("Ambient")
                .withLight(AmbientLight_().withIntensity(0.5f)))
            .with(LightUnit_("Light")
                .withLight(PositionalLight_())
                .withAnimation(Orbiting_(scene::animation::Animation::LOOP)
                    .withDuration(3.0f).withRadius(4.0f).withRotationAxis({0.0f, -1.0f, 0.0f}))
                .withNodeScale(0.15f))
            .with(RenderUnit_("Sphere")
                .withMesh(Sphere())
                .withMaterial(Gold())
                .withAnimation(Rotation_(scene::animation::Animation::LOOP)
                    .withDuration(5.0f).withRotationAxis({1.0f, 0.0f, 1.0f}))
                .withAnimation(Orbiting_(scene::animation::Animation::LOOP)
                    .withDuration(8.0f).withRadius(2.0f).withRotationAxis({0.0f, 1.0f, 0.0f})))
            .with(RenderUnit_("Cube", "Sphere")
                .withMesh("Cube.obj")
                .withMaterial(Silver())
                .withTransform(scene::transform::Scale(0.5f))
                .withAnimation(Orbiting_(scene::animation::Animation::LOOP)
                    .withDuration(4.0f).withRadius(2.0f).withRotationAxis({0.5, 1.5f, -2.0})))
            .with(RenderUnit_("Cylinder", "Cube")
                .withMesh("Cylinder.obj")
                .withMaterial(Bronze())
                .withNodeScale(0.5f)
                .withAnimation(Orbiting_(scene::animation::Animation::LOOP)
                    .withDuration(6.0f).withRadius(1.5f).withRotationAxis({0.5, -1.5f, 2.0}))))
        .build();

        while (!Window::shouldClose()) {
            Input::timeStep();
            Camera::update();
            Renderer::render(scene);
            Window::swapBuffers();
            Input::pollEvents();
        }
        shutDown();
    }


    void Application::shutDown() {
        Window::close();
        glfwTerminate();
    }

} // namespace pixelforge::core
