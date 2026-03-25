#pragma once
#include <concepts>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

namespace pixelforge::geometry { class Mesh; }
namespace pixelforge::scene::nodes {
    class LightUnit;
    class RenderUnit;
    class Model;
    class Scene;
}

namespace pixelforge::core {

    /// Concept constraining types that can be rendered by the engine.
    /// A valid Renderable type must match one of the supported render targets
    /// handled by the Renderer (e.g. Mesh, Model, Scene, etc.).
    /// @tparam T The type to check.
    template <typename T>
    concept Renderable =
        std::same_as<T, geometry::Mesh> ||
        std::same_as<T, scene::nodes::LightUnit> ||
        std::same_as<T, scene::nodes::RenderUnit> ||
        std::same_as<T, scene::nodes::Model> ||
        std::same_as<T, scene::nodes::Scene>;

    /// Manages the lifecycle of the PixelForge application.
    class Application {
    public:
        /// Initializes all systems and renders a user-provided object until the window is closed.
        /// The provided factory must return a `std::shared_ptr<ObjectT>` where `ObjectT` satisfies
        /// the Renderable concept. This allows rendering of scenes, models, or any other
        /// supported renderable type without exposing internal engine components.
        /// @tparam Factory Callable type returning `std::shared_ptr<ObjectT>`.
        /// @param factory A callable that constructs and returns the renderable object.
        /// @note The factory is invoked exactly once before entering the render loop.
        /// @warning The returned type must be one of the supported Renderable types.
        template <typename Factory>
        static void run(Factory&& factory) {
            using PointerT = std::invoke_result_t<Factory&>;
            static_assert(
                std::same_as<PointerT, std::shared_ptr<typename PointerT::element_type>>,
                "Application::run factory must return std::shared_ptr<T>."
            );
            using ObjectT = PointerT::element_type;
            static_assert(
                Renderable<ObjectT>,
                "Application::run factory must return std::shared_ptr of a renderable type."
            );
            runImpl<ObjectT>(std::function<std::shared_ptr<ObjectT>()>(std::forward<Factory>(factory)));
        }

    private:
        /// Internal implementation of the application run loop for a specific renderable type.
        /// This method performs initialization, invokes the factory to construct the object,
        /// and repeatedly updates and renders it until the window is closed.
        /// @tparam ObjectT The concrete renderable type.
        /// @param objectFactory Callable returning the object to render.
        template <Renderable ObjectT>
        static void runImpl(const std::function<std::shared_ptr<ObjectT>()>& objectFactory);

        /// Sets up windowing, rendering, input, camera, and texture management.
        static void initialize();

        /// Closes the window and terminates any backend systems.
        static void shutDown();
    };

} // namespace pixelforge::core
