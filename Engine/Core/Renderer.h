#pragma once
#include <GLM/glm.hpp>
#include <unordered_map>
#include <string>
#include <vector>
class Scene; class Mesh; class LightUnit; class RenderUnit;
class Model; class Shader; class Material;


/// Handles OpenGL state setup and shader management. Can render individual
/// meshes, light units, render units, models, and complete scenes.
class Renderer {
public:
    /// Modes to control how geometry is rasterized.
    enum RenderMode { FILLED, WIREFRAME };

    /// Initializes the renderer's shaders and fallback values.
    static void Initialize();

    /// Renders a specific object using the appropriate draw method.
    /// @tparam ObjectT The type of object to render.
    /// @param object A shared pointer to the object.
    /// @param mode The render mode.
    template <typename ObjectT>
    static void Render(const std::shared_ptr<ObjectT>& object, const RenderMode mode = FILLED) {
        PrepareFrame(mode);
        if constexpr (std::is_same_v<ObjectT, Mesh>) {
            DrawMesh(object);
        } else if constexpr (std::is_same_v<ObjectT, LightUnit>) {
            object->UpdateSelfAndChildren(Input::GetDeltaTime());
            DrawLightUnit(object);
        } else if constexpr (std::is_same_v<ObjectT, RenderUnit>) {
            object->UpdateSelfAndChildren(Input::GetDeltaTime());
            DrawRenderUnit(object);
        } else if constexpr (std::is_same_v<ObjectT, Model>) {
            object->UpdateSelfAndChildren(Input::GetDeltaTime());
            DrawModel(object);
        } else if constexpr (std::is_same_v<ObjectT, Scene>) {
            object->UpdateSelfAndChildren(Input::GetDeltaTime());
            DrawScene(object);
        } else {
            static_assert(false, "Unsupported type in Render.");
        }
    }

private:
    static std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;  ///< Stores all loaded shaders by name.
    static std::shared_ptr<Shader> currentShader; ///< The currently active shader.
    static std::shared_ptr<Material> fallbackMaterial; ///< A default material used when none is specified.
    static std::vector<std::shared_ptr<LightUnit>> fallbackLightUnits;  ///< Light units used as fallback when no scene lights are provided.
    static bool useFallbackLights;  ///< Whether to use fallback lighting instead of scene-defined lights.

    /// Initializes a specific shader by name.
    /// @param name The identifier of the shader to initialize.
    static void InitializeShader(const std::string& name);

    /// Prepares OpenGL state and clears the screen for a new frame.
    /// @param mode FILLED or WIREFRAME.
    /// @param clearColor The background color.
    static void PrepareFrame(RenderMode mode, glm::vec4 clearColor = {0.0f, 0.0f, 0.0f, 1.0f});

    /// Switches to a shader by name if it is not already active.
    /// @param name The identifier for the shader to activate.
    static void SetShader(const std::string& name);

    /// Sets camera-related uniforms for the current shader.
    static void SetCameraUniforms();

    /// Sets lighting uniforms in the current shader.
    /// @param lights A vector of light units to use for setting uniforms.
    static void SetLightingUniforms(const std::vector<std::shared_ptr<LightUnit>>& lights);

    /// Sets material uniforms in the current shader.
    /// @param material The material whose properties should be passed to the shader.
    static void SetMaterialUniforms(const std::shared_ptr<Material>& material);

    /// Renders a mesh using a flat white shader (unlit).
    /// @param mesh The mesh to render.
    static void DrawMesh(const std::shared_ptr<Mesh>& mesh);

    /// Renders a visual representation of a light unit.
    /// @param light The light unit to render.
    static void DrawLightUnit(const std::shared_ptr<LightUnit>& light);

    /// Renders a render unit with full material and lighting.
    /// @param unit The render unit to render.
    static void DrawRenderUnit(const std::shared_ptr<RenderUnit>& unit);

    /// Renders a model and its associated light and render units.
    /// @param model The model to render.
    static void DrawModel(const std::shared_ptr<Model>& model);

    /// Renders an entire scene, including models and lights.
    /// @param scene The scene to render.
    static void DrawScene(const std::shared_ptr<Scene>& scene);

    /// @brief Issues the OpenGL draw call for a mesh using its VAO.
    /// @param mesh The mesh to render.
    static void Draw(const std::shared_ptr<Mesh>& mesh);
};
