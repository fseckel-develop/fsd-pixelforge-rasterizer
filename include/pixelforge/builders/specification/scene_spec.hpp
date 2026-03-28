#pragma once
#include <pixelforge/builders/specification/light_unit_spec.hpp>
#include <pixelforge/builders/specification/model_spec.hpp>
#include <string>
#include <vector>


namespace pixelforge::builders::specification {

    /// Declarative specification for constructing a Scene.
    /// Stores the top-level contents of the scene.
    struct SceneSpec {
        std::string name; ///< Name of the scene.

        std::vector<LightUnitSpec> lightUnits; ///< Top-level light units in the scene.
        std::vector<ModelSpec> models; ///< Top-level models in the scene.
        std::string cubeMapBaseName; ///< Optional cube map base name.
        std::string skySphereTextureFileName; ///< Optional sky sphere texture file name.

        /// Creates a Scene specification with a required name.
        /// @param name Name of the scene.
        explicit SceneSpec(const std::string& name)
            : name(name) {}
    };

} // namespace pixelforge::builders::specification
