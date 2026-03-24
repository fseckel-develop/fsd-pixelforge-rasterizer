#pragma once
#include <pixelforge/core/input.hpp>
#include <GLM/glm.hpp>
#include <unordered_map>
#include <string>
#include <vector>

namespace pixelforge::geometry { class Mesh; }
namespace pixelforge::graphics { class Material; class Shader; }
namespace pixelforge::scene::nodes { class Scene; class LightUnit; class RenderUnit; class Model; }

namespace pixelforge::core {

    template<class> inline constexpr bool always_false_v = false;

    /// Handles OpenGL state setup and shader management. Can render individual
    /// meshes, light units, render units, models, and complete scenes.
    class Renderer {
    public:
        /// Modes to control how geometry is rasterized.
        enum RenderMode { FILLED, WIREFRAME };

        /// Initializes the renderer's shaders and fallback values.
        static void initialize();

        /// Renders a specific object using the appropriate draw method.
        /// @tparam ObjectT The type of object to render.
        /// @param object A shared pointer to the object.
        /// @param mode The render mode.
        template <typename ObjectT>
        static void render(const std::shared_ptr<ObjectT>& object, const RenderMode mode = FILLED) {
            prepareFrame(mode);
            if constexpr (std::is_same_v<ObjectT, geometry::Mesh>) {
                drawMesh(object);
            } else if constexpr (std::is_same_v<ObjectT, scene::nodes::LightUnit>) {
                object->updateSelfAndChildren(Input::getTimeDelta());
                drawLightUnit(object);
            } else if constexpr (std::is_same_v<ObjectT, scene::nodes::RenderUnit>) {
                object->updateSelfAndChildren(Input::getTimeDelta());
                drawRenderUnit(object);
            } else if constexpr (std::is_same_v<ObjectT, scene::nodes::Model>) {
                object->updateSelfAndChildren(Input::getTimeDelta());
                drawModel(object);
            } else if constexpr (std::is_same_v<ObjectT, scene::nodes::Scene>) {
                object->updateSelfAndChildren(Input::getTimeDelta());
                drawScene(object);
            } else {
                static_assert(always_false_v<ObjectT>, "Unsupported type in Renderer::render.");
            }
        }

    private:
        static std::unordered_map<std::string, std::shared_ptr<graphics::Shader>> shaders_;  ///< Stores all loaded shaders by name.
        static std::shared_ptr<graphics::Shader> currentShader_;  ///< The currently active shader.
        static std::shared_ptr<graphics::Material> fallbackMaterial_;  ///< A default material used when none is specified.
        static std::vector<std::shared_ptr<scene::nodes::LightUnit>> fallbackLightUnits_;  ///< Light units used as fallback when scene has no lights.
        static bool useFallbackLights_;  ///< Whether to use fallback lighting instead of scene-defined lights.

        /// Initializes a specific shader by name.
        /// @param name The identifier of the shader to initialize.
        static void initializeShader(const std::string& name);

        /// Prepares OpenGL state and clears the screen for a new frame.
        /// @param mode FILLED or WIREFRAME.
        /// @param clearColor The background color.
        static void prepareFrame(RenderMode mode, glm::vec4 clearColor = {0.0f, 0.0f, 0.0f, 1.0f});

        /// Switches to a shader by name if it is not already active.
        /// @param name The identifier for the shader to activate.
        static void setShader(const std::string& name);

        /// Sets camera-related uniforms for the current shader.
        static void setCameraUniforms();

        /// Sets lighting uniforms in the current shader.
        /// @param lights A vector of light units to use for setting uniforms.
        static void setLightingUniforms(const std::vector<std::shared_ptr<scene::nodes::LightUnit>>& lights);

        /// Sets material uniforms in the current shader.
        /// @param material The material whose properties should be passed to the shader.
        static void setMaterialUniforms(const std::shared_ptr<graphics::Material>& material);

        /// Renders a mesh using a flat white shader (unlit).
        /// @param mesh The mesh to render.
        static void drawMesh(const std::shared_ptr<geometry::Mesh>& mesh);

        /// Renders a visual representation of a light unit.
        /// @param light The light unit to render.
        static void drawLightUnit(const std::shared_ptr<scene::nodes::LightUnit>& light);

        /// Renders a render unit with full material and lighting.
        /// @param unit The render unit to render.
        static void drawRenderUnit(const std::shared_ptr<scene::nodes::RenderUnit>& unit);

        /// Renders a model and its associated light and render units.
        /// @param model The model to render.
        static void drawModel(const std::shared_ptr<scene::nodes::Model>& model);

        /// Renders an entire scene, including models and lights.
        /// @param scene The scene to render.
        static void drawScene(const std::shared_ptr<scene::nodes::Scene>& scene);

        /// @brief Issues the OpenGL draw call for a mesh using its VAO.
        /// @param mesh The mesh to render.
        static void draw(const std::shared_ptr<geometry::Mesh>& mesh);
    };

} // namespace pixelforge::core
