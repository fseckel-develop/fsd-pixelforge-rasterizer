#define GLEW_STATIC
#include <pixelforge/core/application.hpp>
#include <pixelforge/core/window.hpp>
#include <pixelforge/core/camera.hpp>
#include <pixelforge/core/context.hpp>
#include <pixelforge/geometry/meshes/mesh.hpp>
#include <pixelforge/scene/nodes/light_unit.hpp>
#include <pixelforge/scene/nodes/render_unit.hpp>
#include <pixelforge/scene/nodes/model.hpp>
#include <pixelforge/scene/nodes/scene.hpp>
#include "core/input.hpp"
#include "core/renderer.hpp"
#include "managers/texture_manager.hpp"


namespace pixelforge::core {

    void Application::initialize() {
        context::initializeGLFW();
        Window::create(1400, 900, "PixelForge");
        context::initializeGLEW();
        Camera::turnOn({0.0f, 0.0f, 5.0f});
        Input::enable();
        Renderer::initialize();
        management::TextureManager::initialize();
    }


    template <Renderable ObjectT>
    void Application::runImpl(const std::function<std::shared_ptr<ObjectT>()>& objectFactory) {
        initialize();
        {
            const auto object = objectFactory();
            while (!Window::shouldClose()) {
                Input::timeStep();
                Camera::update();
                Renderer::render(object);
                Window::swapBuffers();
                Input::pollEvents();
            }
        }
        shutDown();
    }


    void Application::shutDown() {
        Window::close();
        glfwTerminate();
    }

} // namespace pixelforge::core


// Explicit template instantiations
template void pixelforge::core::Application::runImpl<pixelforge::geometry::Mesh>(
    const std::function<std::shared_ptr<geometry::Mesh>()>&);

template void pixelforge::core::Application::runImpl<pixelforge::scene::nodes::LightUnit>(
    const std::function<std::shared_ptr<scene::nodes::LightUnit>()>&);

template void pixelforge::core::Application::runImpl<pixelforge::scene::nodes::RenderUnit>(
    const std::function<std::shared_ptr<scene::nodes::RenderUnit>()>&);

template void pixelforge::core::Application::runImpl<pixelforge::scene::nodes::Model>(
    const std::function<std::shared_ptr<scene::nodes::Model>()>&);

template void pixelforge::core::Application::runImpl<pixelforge::scene::nodes::Scene>(
    const std::function<std::shared_ptr<scene::nodes::Scene>()>&);
