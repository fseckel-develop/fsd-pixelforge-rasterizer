#pragma once
#include <memory>
#include <GLM/glm.hpp>
class Light;


/// Concept to validate if a type implements a Build()
/// method which returns a shared pointer to a light.
template<typename T>
concept IsLightBuilder = requires(T t) {
    { t.Build() } -> std::convertible_to<std::shared_ptr<Light>>;
};


/// Base class template for creating fluent-style builders for specific light types.
/// @tparam LightT The specific Light subclass to build.
/// @tparam DerivedBuilder The concrete builder class inheriting from this template.
template<typename LightT, typename DerivedBuilder>
class LightBuilder {
public:
    /// Starts the building process for a light.
    LightBuilder() {
        light = std::make_shared<LightT>();
    }

    /// Sets the overall color of the light being built.
    /// @param color The RGB color.
    /// @return Reference to the derived builder for method chaining.
    auto& withColor(const glm::vec3& color) {
        light->SetColor(color);
        return static_cast<DerivedBuilder&>(*this);
    }

    /// Sets the overall intensity of the light being built.
    /// @param intensity Intensity value between 0.0 and 1.0.
    /// @return Reference to the derived builder for method chaining.
    auto& withIntensity(const float intensity) {
        light->SetIntensity(intensity);
        return static_cast<DerivedBuilder&>(*this);
    }

    /// Sets color and intensity of the ambient component.
    /// @param color The RGB color.
    /// @param intensity Intensity value between 0.0 and 1.0.
    /// @return Reference to the derived builder for method chaining.
    auto& withAmbient(const glm::vec3& color, const float intensity) {
        light->SetAmbient(color, intensity);
        return static_cast<DerivedBuilder&>(*this);
    }

    /// Sets color and intensity of the diffuse component.
    /// @param color The RGB color.
    /// @param intensity Intensity value between 0.0 and 1.0.
    /// @return Reference to the derived builder for method chaining.
    auto& withDiffuse(const glm::vec3& color, const float intensity) {
        light->SetDiffuse(color, intensity);
        return static_cast<DerivedBuilder&>(*this);
    }

    /// Sets color and intensity of the specular component.
    /// @param color The RGB color.
    /// @param intensity Intensity value between 0.0 and 1.0.
    /// @return Reference to the derived builder for method chaining.
    auto& withSpecular(const glm::vec3& color, const float intensity) {
        light->SetSpecular(color, intensity);
        return static_cast<DerivedBuilder&>(*this);
    }

    /// Finalizes and returns the constructed light object.
    /// @return Shared pointer to the configured light.
    std::shared_ptr<LightT> Build() const {
        return light;
    }

    /// Implicit conversion to allow the builder to be used as a light.
    /// @return Shared pointer to the constructed light.
    operator std::shared_ptr<LightT>() { // NOLINT
        return Build();
    }

protected:
    std::shared_ptr<LightT> light; ///< The underlying light being built.
};
