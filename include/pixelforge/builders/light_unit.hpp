#pragma once
#include <pixelforge/builders/animation.hpp>
#include <pixelforge/builders/light.hpp>
#include <pixelforge/builders/specification/light_unit_spec.hpp>

namespace pixelforge::scene::nodes { class LightUnit; }
namespace pixelforge::builders::specification { struct LightUnitSpec; }
namespace pixelforge::scene::builders::compiler {
    std::shared_ptr<nodes::LightUnit> buildLightUnit(
        const pixelforge::builders::specification::LightUnitSpec& spec
    );
}

namespace pixelforge::builders {

    /// Fluent nested-lambda builder for configuring a LightUnitSpec.
    class LightUnit final {
    public:
        /// Starts the building process for a light unit with a name and parent (optional).
        /// @param name The name of the light unit.
        /// @param parentName The optional name of the parent node.
        explicit LightUnit(const std::string& name, const std::string& parentName = ""):
            spec_(name) {
            spec_.parentName = parentName;
        }

        /// Sets the name of the light unit.
        /// @param newName The new name.
        /// @return Reference to the current builder for fluent chaining.
        auto& name(const std::string& newName) {
            spec_.name = newName;
            return *this;
        }

        /// Sets the parent name of the light unit.
        /// @param parentName The parent node name.
        /// @return Reference to the current builder for fluent chaining.
        auto& parent(const std::string& parentName) {
            spec_.parentName = parentName;
            return *this;
        }

        /// Sets the light specification directly.
        /// @param light The light specification to assign.
        /// @return Reference to the current builder for fluent chaining.
        auto& light(const specification::LightSpec& light) {
            spec_.light = light;
            return *this;
        }

        /// Sets the light using an ambient light builder.
        /// @param builder The light builder.
        /// @return Reference to the current builder for fluent chaining.
        auto& light(const AmbientLight& builder) {
            spec_.light = builder.buildSpec();
            return *this;
        }

        /// Sets the light using a directional light builder.
        /// @param builder The light builder.
        /// @return Reference to the current builder for fluent chaining.
        auto& light(const DirectionalLight& builder) {
            spec_.light = builder.buildSpec();
            return *this;
        }

        /// Sets the light using a positional light builder.
        /// @param builder The light builder.
        /// @return Reference to the current builder for fluent chaining.
        auto& light(const PositionalLight& builder) {
            spec_.light = builder.buildSpec();
            return *this;
        }

        /// Sets the light using a spotlight builder.
        /// @param builder The light builder.
        /// @return Reference to the current builder for fluent chaining.
        auto& light(const SpotLight& builder) {
            spec_.light = builder.buildSpec();
            return *this;
        }

        /// Sets the mesh used to visualize this light unit.
        /// @param mesh The mesh to assign.
        /// @return Reference to the current builder for fluent chaining.
        auto& mesh(const std::shared_ptr<geometry::Mesh>& mesh) {
            spec_.mesh = mesh;
            return *this;
        }

        /// Sets the transform for this light unit.
        /// @param transform The transform to assign.
        /// @return Reference to the current builder for fluent chaining.
        auto& transform(const scene::transform::Transform& transform) {
            spec_.transform = transform;
            return *this;
        }

        /// Sets a uniform node scale for this light unit.
        /// @param nodeScale The uniform scale factor.
        /// @return Reference to the current builder for fluent chaining.
        auto& nodeScale(float nodeScale) {
            spec_.uniformNodeScale = nodeScale;
            spec_.nodeScale.reset();
            return *this;
        }

        /// Sets a non-uniform node scale for this light unit.
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

        /// Finalizes the build process and returns the light unit specification.
        /// @return LightUnitSpec describing the configured light unit.
        [[nodiscard]] specification::LightUnitSpec buildSpec() const {
            return spec_;
        }

        /// Compiles the light unit specification into a runtime LightUnit object.
        /// @return Shared pointer to the constructed runtime light unit.
        [[nodiscard]] std::shared_ptr<scene::nodes::LightUnit> build() const {
            return scene::builders::compiler::buildLightUnit(spec_);
        }

        /// Implicit conversion to allow the builder to be used as a runtime LightUnit.
        /// @return Shared pointer to the constructed runtime light unit.
        operator std::shared_ptr<scene::nodes::LightUnit>() const { // NOLINT
            return build();
        }

    private:
        specification::LightUnitSpec spec_; ///< The underlying light unit specification being built.
    };

} // namespace pixelforge::builders
