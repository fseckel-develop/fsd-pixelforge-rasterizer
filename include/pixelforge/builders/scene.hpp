#pragma once
#include <pixelforge/builders/light_unit.hpp>
#include <pixelforge/builders/model.hpp>
#include <pixelforge/builders/specification/scene_spec.hpp>
#include <concepts>
#include <utility>

namespace pixelforge::scene::nodes { class Scene; }
namespace pixelforge::builders::specification { struct SceneSpec; }
namespace pixelforge::scene::builders::compiler {
    std::shared_ptr<nodes::Scene> buildScene(const pixelforge::builders::specification::SceneSpec& spec);
}

namespace pixelforge::builders {

    /// Fluent nested-lambda builder for configuring a SceneSpec.
    class Scene final {
    public:
        /// Starts the building process for a scene with a name.
        /// @param name The name of the scene.
        explicit Scene(const std::string& name):
            spec_(name) {
        }

        /// Sets the name of the scene.
        /// @param newName The new name.
        /// @return Reference to the current builder for fluent chaining.
        auto& name(const std::string& newName) {
            spec_.name = newName;
            return *this;
        }

        /// Adds a nested top-level light unit using a configuration lambda.
        /// @tparam ConfigureT The lambda/function type.
        /// @param name The name of the light unit.
        /// @param configure Lambda that configures the nested builder.
        /// @return Reference to the current builder for fluent chaining.
        template<typename ConfigureT>
        requires std::invocable<ConfigureT, LightUnit&>
        auto& lightUnit(const std::string& name, ConfigureT&& configure) {
            LightUnit builder(name);
            std::forward<ConfigureT>(configure)(builder);
            spec_.lightUnits.push_back(builder.buildSpec());
            return *this;
        }

        /// Adds a prebuilt top-level light unit specification.
        /// @param lightUnit The light unit specification to add.
        /// @return Reference to the current builder for fluent chaining.
        auto& add(const specification::LightUnitSpec& lightUnit) {
            spec_.lightUnits.push_back(lightUnit);
            return *this;
        }

        /// Adds a nested model using a configuration lambda.
        /// @tparam ConfigureT The lambda/function type.
        /// @param name The name of the model.
        /// @param configure Lambda that configures the nested builder.
        /// @return Reference to the current builder for fluent chaining.
        template<typename ConfigureT>
        requires std::invocable<ConfigureT, Model&>
        auto& model(const std::string& name, ConfigureT&& configure) {
            Model builder(name);
            std::forward<ConfigureT>(configure)(builder);
            spec_.models.push_back(builder.buildSpec());
            return *this;
        }

        /// Adds a prebuilt model specification.
        /// @param model The model specification to add.
        /// @return Reference to the current builder for fluent chaining.
        auto& add(const specification::ModelSpec& model) {
            spec_.models.push_back(model);
            return *this;
        }

        /// Sets a cube map file set to be used for this scene.
        /// @param baseName The base name of the cube map file set.
        /// @return Reference to the current builder for fluent chaining.
        auto& cubeMap(const std::string& baseName) {
            spec_.cubeMapBaseName = baseName;
            spec_.skySphereTextureFileName.clear();
            return *this;
        }

        /// Sets a sky sphere texture file to be used for this scene.
        /// @param fileName The file name of the sky sphere texture.
        /// @return Reference to the current builder for fluent chaining.
        auto& skySphere(const std::string& fileName) {
            spec_.skySphereTextureFileName = fileName;
            spec_.cubeMapBaseName.clear();
            return *this;
        }

        /// Finalizes the configuration and returns the scene specification.
        /// @return SceneSpec describing the configured scene.
        [[nodiscard]] specification::SceneSpec buildSpec() const {
            return spec_;
        }

        /// Compiles the scene specification into a runtime Scene object.
        /// @return Shared pointer to the constructed runtime scene.
        [[nodiscard]] std::shared_ptr<scene::nodes::Scene> build() const {
            return scene::builders::compiler::buildScene(spec_);
        }

        /// Implicit conversion to allow the builder to be used as a runtime Scene.
        /// @return Shared pointer to the constructed runtime scene.
        operator std::shared_ptr<scene::nodes::Scene>() const { // NOLINT
            return build();
        }

    private:
        specification::SceneSpec spec_; ///< The underlying scene specification being built.
    };

} // namespace pixelforge::builders
