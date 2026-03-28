#pragma once
#include <pixelforge/builders/animation.hpp>
#include <pixelforge/builders/light_unit.hpp>
#include <pixelforge/builders/render_unit.hpp>
#include <pixelforge/builders/specification/model_spec.hpp>
#include <concepts>
#include <utility>

namespace pixelforge::scene::nodes { class Model; }
namespace pixelforge::builders::specification { struct ModelSpec; }
namespace pixelforge::scene::builders::compiler {
    std::shared_ptr<nodes::Model> buildModel(
        const pixelforge::builders::specification::ModelSpec& spec
    );
}

namespace pixelforge::builders {

    /// Fluent nested-lambda builder for configuring a ModelSpec.
    class Model final {
    public:
        /// Starts the building process for a model with a name and parent (optional).
        /// @param name The name of the model.
        /// @param parentName The optional name of the parent node.
        explicit Model(const std::string& name, const std::string& parentName = ""):
            spec_(name) {
            spec_.parentName = parentName;
        }

        /// Sets the name of the model.
        /// @param newName The new name.
        /// @return Reference to the current builder for fluent chaining.
        auto& name(const std::string& newName) {
            spec_.name = newName;
            return *this;
        }

        /// Sets the parent name of the model.
        /// @param parentName The parent node name.
        /// @return Reference to the current builder for fluent chaining.
        auto& parent(const std::string& parentName) {
            spec_.parentName = parentName;
            return *this;
        }

        /// Sets the transform for this model.
        /// @param transform The transform to assign.
        /// @return Reference to the current builder for fluent chaining.
        auto& transform(const scene::transform::Transform& transform) {
            spec_.transform = transform;
            return *this;
        }

        /// Sets a uniform node scale for this model.
        /// @param nodeScale The uniform scale factor.
        /// @return Reference to the current builder for fluent chaining.
        auto& nodeScale(const float nodeScale) {
            spec_.uniformNodeScale = nodeScale;
            spec_.nodeScale.reset();
            return *this;
        }

        /// Sets a non-uniform node scale for this model.
        /// @param nodeScale The scale vector.
        /// @return Reference to the current builder for fluent chaining.
        auto& nodeScale(const glm::vec3& nodeScale) {
            spec_.nodeScale = nodeScale;
            spec_.uniformNodeScale.reset();
            return *this;
        }

        /// Adds an animation specification directly.
        /// @param animation The animation specification to add.
        /// @return Reference to the current builder for fluent chaining.
        auto& animation(const specification::AnimationSpec& animation) {
            spec_.animations.push_back(animation);
            return *this;
        }

        /// Adds a rotation animation using a builder.
        /// @param builder The animation builder.
        /// @return Reference to the current builder for fluent chaining.
        auto& animation(const Rotation& builder) {
            spec_.animations.push_back(builder.buildSpec());
            return *this;
        }

        /// Adds an orbiting animation using a builder.
        /// @param builder The animation builder.
        /// @return Reference to the current builder for fluent chaining.
        auto& animation(const Orbiting& builder) {
            spec_.animations.push_back(builder.buildSpec());
            return *this;
        }

        /// Adds an elliptic orbiting animation using a builder.
        /// @param builder The animation builder.
        /// @return Reference to the current builder for fluent chaining.
        auto& animation(const EllipticOrbiting& builder) {
            spec_.animations.push_back(builder.buildSpec());
            return *this;
        }

        /// Adds a scaling animation using a builder.
        /// @param builder The animation builder.
        /// @return Reference to the current builder for fluent chaining.
        auto& animation(const Scaling& builder) {
            spec_.animations.push_back(builder.buildSpec());
            return *this;
        }

        /// Adds a translation animation using a builder.
        /// @param builder The animation builder.
        /// @return Reference to the current builder for fluent chaining.
        auto& animation(const Translation& builder) {
            spec_.animations.push_back(builder.buildSpec());
            return *this;
        }

        /// Adds a keyframe animation using a builder.
        /// @param builder The animation builder.
        /// @return Reference to the current builder for fluent chaining.
        auto& animation(const KeyframeAnimation& builder) {
            spec_.animations.push_back(builder.buildSpec());
            return *this;
        }

        /// Adds a curve animation using a builder.
        /// @param builder The animation builder.
        /// @return Reference to the current builder for fluent chaining.
        auto& animation(const CurveAnimation& builder) {
            spec_.animations.push_back(builder.buildSpec());
            return *this;
        }

        /// Adds a nested light unit using a configuration lambda.
        /// @tparam ConfigureFn The lambda/function type.
        /// @param name The name of the light unit.
        /// @param configure Lambda that configures the nested builder.
        /// @return Reference to the current builder for fluent chaining.
        template<typename ConfigureFn>
        requires std::invocable<ConfigureFn, LightUnit&>
        auto& lightUnit(const std::string& name, ConfigureFn&& configure) {
            LightUnit builder(name);
            std::forward<ConfigureFn>(configure)(builder);
            spec_.lightUnits.push_back(builder.buildSpec());
            return *this;
        }

        /// Adds a prebuilt light unit specification.
        /// @param lightUnit The light unit specification to add.
        /// @return Reference to the current builder for fluent chaining.
        auto& add(const specification::LightUnitSpec& lightUnit) {
            spec_.lightUnits.push_back(lightUnit);
            return *this;
        }

        /// Adds a nested render unit using a configuration lambda.
        /// @tparam ConfigureFn The lambda/function type.
        /// @param name The name of the render unit.
        /// @param configure Lambda that configures the nested builder.
        /// @return Reference to the current builder for fluent chaining.
        template<typename ConfigureFn>
        requires std::invocable<ConfigureFn, RenderUnit&>
        auto& renderUnit(const std::string& name, ConfigureFn&& configure) {
            RenderUnit builder(name);
            std::forward<ConfigureFn>(configure)(builder);
            spec_.renderUnits.push_back(builder.buildSpec());
            return *this;
        }

        /// Adds a prebuilt render unit specification.
        /// @param renderUnit The render unit specification to add.
        /// @return Reference to the current builder for fluent chaining.
        auto& add(const specification::RenderUnitSpec& renderUnit) {
            spec_.renderUnits.push_back(renderUnit);
            return *this;
        }

        /// Finalizes the build process and returns the model specification.
        /// @return ModelSpec describing the configured model.
        [[nodiscard]] specification::ModelSpec buildSpec() const {
            return spec_;
        }

        /// Compiles the model specification into a runtime Model object.
        /// @return Shared pointer to the constructed runtime model.
        [[nodiscard]] std::shared_ptr<scene::nodes::Model> build() const {
            return scene::builders::compiler::buildModel(spec_);
        }

        /// Implicit conversion to allow the builder to be used as a runtime Model.
        /// @return Shared pointer to the constructed runtime model.
        operator std::shared_ptr<scene::nodes::Model>() const { // NOLINT
            return build();
        }

    private:
        specification::ModelSpec spec_; ///< The underlying model specification being built.
    };

} // namespace pixelforge::builders
