#pragma once
#include <pixelforge/builders/animation.hpp>
#include <pixelforge/builders/specification/render_unit_spec.hpp>

namespace pixelforge::scene::nodes { class RenderUnit; }
namespace pixelforge::builders::specification { struct RenderUnitSpec; }
namespace pixelforge::scene::builders::compiler {
    std::shared_ptr<nodes::RenderUnit> buildRenderUnit(
        const pixelforge::builders::specification::RenderUnitSpec& spec
    );
}

namespace pixelforge::builders {

    /// Fluent nested-lambda builder for configuring a RenderUnitSpec.
    class RenderUnit final {
    public:
        /// Starts the building process for a render unit with a name and parent (optional).
        /// @param name The name of the render unit.
        /// @param parentName The optional name of the parent node.
        explicit RenderUnit(const std::string& name, const std::string& parentName = ""):
            spec_(name) {
            spec_.parentName = parentName;
        }

        /// Sets the name of the render unit.
        /// @param newName The new name.
        /// @return Reference to the current builder for fluent chaining.
        auto& name(const std::string& newName) {
            spec_.name = newName;
            return *this;
        }

        /// Sets the parent name of the render unit.
        /// @param parentName The parent node name.
        /// @return Reference to the current builder for fluent chaining.
        auto& parent(const std::string& parentName) {
            spec_.parentName = parentName;
            return *this;
        }

        /// Sets the mesh from a shared mesh instance.
        /// @param mesh The mesh to assign.
        /// @return Reference to the current builder for fluent chaining.
        auto& mesh(const std::shared_ptr<geometry::Mesh>& mesh) {
            spec_.mesh = mesh;
            spec_.meshFileName.clear();
            return *this;
        }

        /// Sets the mesh from a mesh value.
        /// @param mesh The mesh to copy.
        /// @return Reference to the current builder for fluent chaining.
        auto& mesh(const geometry::Mesh& mesh) {
            spec_.mesh = std::make_shared<geometry::Mesh>(mesh);
            spec_.meshFileName.clear();
            return *this;
        }

        /// Sets the mesh from a file name.
        /// @param fileName The .obj file name.
        /// @return Reference to the current builder for fluent chaining.
        auto& mesh(const std::string& fileName) {
            spec_.mesh = nullptr;
            spec_.meshFileName = fileName;
            return *this;
        }

        /// Sets the material from a shared material instance.
        /// @param material The material to assign.
        /// @return Reference to the current builder for fluent chaining.
        auto& material(const std::shared_ptr<graphics::Material>& material) {
            spec_.material = material;
            spec_.textureFileName.clear();
            return *this;
        }

        /// Sets the material from a material value.
        /// @param material The material to copy.
        /// @return Reference to the current builder for fluent chaining.
        auto& material(const graphics::Material& material) {
            spec_.material = std::make_shared<graphics::Material>(material);
            spec_.textureFileName.clear();
            return *this;
        }

        /// Sets a texture file to be used for this render unit.
        /// @param fileName The texture file name.
        /// @return Reference to the current builder for fluent chaining.
        auto& texture(const std::string& fileName) {
            spec_.textureFileName = fileName;
            return *this;
        }

        /// Sets the transform for this render unit.
        /// @param transform The transform to assign.
        /// @return Reference to the current builder for fluent chaining.
        auto& transform(const scene::transform::Transform& transform) {
            spec_.transform = transform;
            return *this;
        }

        /// Sets a uniform node scale for this render unit.
        /// @param nodeScale The uniform scale factor.
        /// @return Reference to the current builder for fluent chaining.
        auto& nodeScale(const float nodeScale) {
            spec_.uniformNodeScale = nodeScale;
            spec_.nodeScale.reset();
            return *this;
        }

        /// Sets a non-uniform node scale for this render unit.
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

        /// Finalizes the build process and returns the render unit specification.
        /// @return RenderUnitSpec describing the configured render unit.
        [[nodiscard]] specification::RenderUnitSpec buildSpec() const {
            return spec_;
        }

        /// Compiles the render unit specification into a runtime RenderUnit object.
        /// @return Shared pointer to the constructed runtime render unit.
        [[nodiscard]] std::shared_ptr<scene::nodes::RenderUnit> build() const {
            return scene::builders::compiler::buildRenderUnit(spec_);
        }

        /// Implicit conversion to allow the builder to be used as a runtime RenderUnit.
        /// @return Shared pointer to the constructed runtime render unit.
        operator std::shared_ptr<scene::nodes::RenderUnit>() const { // NOLINT
            return build();
        }

    private:
        specification::RenderUnitSpec spec_; ///< The underlying render unit specification being built.
    };

} // namespace pixelforge::builders
