#pragma once
#include <pixelforge/builders/specification/light_spec.hpp>


namespace pixelforge::builders {

    /// Common base class for declarative light builders.
    /// Provides shared helper functionality for managing light components
    /// and finalizing the build process into a LightSpec.
    class LightBuilderBase {
    public:
        /// Finalizes the build process and returns the light specification.
        /// @return Declarative specification of the configured light.
        [[nodiscard]] specification::LightSpec buildSpec() const {
            return spec_;
        }

    protected:
        specification::LightSpec spec_; ///< The underlying light specification being built.

        /// Creates a default light component with white color and full intensity.
        /// @return Default-initialized light component.
        [[nodiscard]] static scene::lighting::LightComponent defaultComponent() {
            return {glm::vec3(1.0f), 1.0f};
        }

        /// Ensures that an optional light component is initialized.
        /// @param component The component to initialize if empty.
        static void ensureComponent(std::optional<scene::lighting::LightComponent>& component) {
            if (!component) component = defaultComponent();
        }

        /// Sets the ambient component of a light specification.
        /// @tparam LightSpecT The concrete light specification type.
        /// @param color The RGB color to apply.
        /// @param intensity Intensity value between 0.0 and 1.0.
        /// @return Reference to this builder for method chaining.
        template<typename LightSpecT, typename DerivedT>
        auto& setAmbient(const glm::vec3& color, const float intensity) {
            auto& data = std::get<LightSpecT>(spec_.data);
            data.ambient = scene::lighting::LightComponent{color, intensity};
            return static_cast<DerivedT&>(*this);
        }

        /// Sets the diffuse component of a light specification.
        /// @tparam LightSpecT The concrete light specification type.
        /// @param color The RGB color to apply.
        /// @param intensity Intensity value between 0.0 and 1.0.
        /// @return Reference to this builder for method chaining.
        template<typename LightSpecT, typename DerivedT>
        auto& setDiffuse(const glm::vec3& color, const float intensity) {
            auto& data = std::get<LightSpecT>(spec_.data);
            data.diffuse = scene::lighting::LightComponent{color, intensity};
            return static_cast<DerivedT&>(*this);
        }

        /// Sets the specular component of a light specification.
        /// @tparam LightSpecT The concrete light specification type.
        /// @param color The RGB color to apply.
        /// @param intensity Intensity value between 0.0 and 1.0.
        /// @return Reference to this builder for method chaining.
        template<typename LightSpecT, typename DerivedT>
        auto& setSpecular(const glm::vec3& color, const float intensity) {
            auto& data = std::get<LightSpecT>(spec_.data);
            data.specular = scene::lighting::LightComponent{color, intensity};
            return static_cast<DerivedT&>(*this);
        }

        /// Sets the RGB color for all available lighting components of a light specification.
        /// Ambient is always affected; diffuse and specular are updated only if the
        /// concrete light specification provides them.
        /// @tparam LightSpecT The concrete light specification type.
        /// @return Reference to this builder for method chaining.
        template<typename LightSpecT, typename DerivedT>
        auto& setColor(const glm::vec3& color) {
            auto& data = std::get<LightSpecT>(spec_.data);
            ensureComponent(data.ambient);
            data.ambient->color = color;
            if constexpr (requires { data.diffuse; }) {
                ensureComponent(data.diffuse);
                data.diffuse->color = color;
            }
            if constexpr (requires { data.specular; }) {
                ensureComponent(data.specular);
                data.specular->color = color;
            }
            return static_cast<DerivedT&>(*this);
        }

        /// Sets the intensity for all available lighting components of a light specification.
        /// Ambient is always affected; diffuse and specular are updated only if the
        /// concrete light specification provides them.
        /// @tparam LightSpecT The concrete light specification type.
        /// @return Reference to this builder for method chaining.
        template<typename LightSpecT, typename DerivedT>
        auto& setIntensity(const float intensity) {
            auto& data = std::get<LightSpecT>(spec_.data);
            ensureComponent(data.ambient);
            data.ambient->intensity = intensity;
            if constexpr (requires { data.diffuse; }) {
                ensureComponent(data.diffuse);
                data.diffuse->intensity = intensity;
            }
            if constexpr (requires { data.specular; }) {
                ensureComponent(data.specular);
                data.specular->intensity = intensity;
            }
            return static_cast<DerivedT&>(*this);
        }
    };


    /// Fluent builder for constructing an ambient light specification.
    class AmbientLight final : public LightBuilderBase {
    public:
        /// Starts the building process for an ambient light.
        AmbientLight() {
            spec_.data = specification::AmbientLightSpec{};
        }

        /// Sets the RGB color for the ambient component.
        /// @param color The RGB color to apply.
        /// @return Reference to this builder for method chaining.
        auto& color(const glm::vec3& color) {
            return setColor<specification::AmbientLightSpec, AmbientLight>(color);
        }

        /// Sets the intensity for the ambient component.
        /// @param intensity Intensity value between 0.0 and 1.0.
        /// @return Reference to this builder for method chaining.
        auto& intensity(const float intensity) {
            return setIntensity<specification::AmbientLightSpec, AmbientLight>(intensity);
        }

        /// Sets color and intensity of the ambient component.
        /// @param color The RGB color to apply.
        /// @param intensity Intensity value between 0.0 and 1.0.
        /// @return Reference to this builder for method chaining.
        auto& ambient(const glm::vec3& color, const float intensity) {
            return setAmbient<specification::AmbientLightSpec, AmbientLight>(color, intensity);
        }
    };


    /// Fluent builder for constructing a directional light specification.
    class DirectionalLight final : public LightBuilderBase {
    public:
        /// Starts the building process for a directional light.
        DirectionalLight() {
            spec_.data = specification::DirectionalLightSpec{};
        }

        /// Sets the RGB color for all lighting components.
        /// @param color The RGB color to apply.
        /// @return Reference to this builder for method chaining.
        auto& color(const glm::vec3& color) {
            return setColor<specification::DirectionalLightSpec, DirectionalLight>(color);
        }

        /// Sets the intensity for all lighting components.
        /// @param intensity Intensity value between 0.0 and 1.0.
        /// @return Reference to this builder for method chaining.
        auto& intensity(const float intensity) {
            return setIntensity<specification::DirectionalLightSpec, DirectionalLight>(intensity);
        }

        /// Sets color and intensity of the ambient component.
        /// @param color The RGB color to apply.
        /// @param intensity Intensity value between 0.0 and 1.0.
        /// @return Reference to this builder for method chaining.
        auto& ambient(const glm::vec3& color, const float intensity) {
            return setAmbient<specification::DirectionalLightSpec, DirectionalLight>(color, intensity);
        }

        /// Sets color and intensity of the diffuse component.
        /// @param color The RGB color to apply.
        /// @param intensity Intensity value between 0.0 and 1.0.
        /// @return Reference to this builder for method chaining.
        auto& diffuse(const glm::vec3& color, const float intensity) {
            return setDiffuse<specification::DirectionalLightSpec, DirectionalLight>(color, intensity);
        }

        /// Sets color and intensity of the specular component.
        /// @param color The RGB color to apply.
        /// @param intensity Intensity value between 0.0 and 1.0.
        /// @return Reference to this builder for method chaining.
        auto& specular(const glm::vec3& color, const float intensity) {
            return setSpecular<specification::DirectionalLightSpec, DirectionalLight>(color, intensity);
        }

        /// Sets the direction of the directional light being built.
        /// @param direction The direction vector for the light.
        /// @return Reference to this builder for method chaining.
        auto& direction(const glm::vec3& direction) {
            std::get<specification::DirectionalLightSpec>(spec_.data).direction = direction;
            return *this;
        }
    };


    /// Fluent builder for constructing a positional light specification.
    class PositionalLight final : public LightBuilderBase {
    public:
        /// Starts the building process for a positional light.
        PositionalLight() {
            spec_.data = specification::PositionalLightSpec{};
        }

        /// Sets the RGB color for all lighting components.
        /// @param color The RGB color to apply.
        /// @return Reference to this builder for method chaining.
        auto& color(const glm::vec3& color) {
            return setColor<specification::PositionalLightSpec, PositionalLight>(color);
        }

        /// Sets the intensity for all lighting components.
        /// @param intensity Intensity value between 0.0 and 1.0.
        /// @return Reference to this builder for method chaining.
        auto& intensity(const float intensity) {
            return setIntensity<specification::PositionalLightSpec, PositionalLight>(intensity);
        }

        /// Sets color and intensity of the ambient component.
        /// @param color The RGB color to apply.
        /// @param intensity Intensity value between 0.0 and 1.0.
        /// @return Reference to this builder for method chaining.
        auto& ambient(const glm::vec3& color, const float intensity) {
            return setAmbient<specification::PositionalLightSpec, PositionalLight>(color, intensity);
        }

        /// Sets color and intensity of the diffuse component.
        /// @param color The RGB color to apply.
        /// @param intensity Intensity value between 0.0 and 1.0.
        /// @return Reference to this builder for method chaining.
        auto& diffuse(const glm::vec3& color, const float intensity) {
            return setDiffuse<specification::PositionalLightSpec, PositionalLight>(color, intensity);
        }

        /// Sets color and intensity of the specular component.
        /// @param color The RGB color to apply.
        /// @param intensity Intensity value between 0.0 and 1.0.
        /// @return Reference to this builder for method chaining.
        auto& specular(const glm::vec3& color, const float intensity) {
            return setSpecular<specification::PositionalLightSpec, PositionalLight>(color, intensity);
        }

        /// Sets the position of the positional light being built.
        /// @param position The position vector for the light.
        /// @return Reference to this builder for method chaining.
        auto& position(const glm::vec3& position) {
            std::get<specification::PositionalLightSpec>(spec_.data).position = position;
            return *this;
        }

        /// Sets the attenuation directly for the positional light being built.
        /// @param attenuation The attenuation factors (constant, linear, quadratic).
        /// @return Reference to this builder for method chaining.
        auto& attenuation(const scene::lighting::Attenuation& attenuation) {
            auto& data = std::get<specification::PositionalLightSpec>(spec_.data);
            data.attenuation = attenuation;
            data.range.reset();
            return *this;
        }

        /// Sets the attenuation based on range for the positional light being built.
        /// @param range The range of the light.
        /// @return Reference to this builder for method chaining.
        auto& range(const scene::lighting::Range range) {
            auto& data = std::get<specification::PositionalLightSpec>(spec_.data);
            data.range = range;
            data.attenuation.reset();
            return *this;
        }
    };


    /// Fluent builder for constructing a spotlight specification.
    class SpotLight final : public LightBuilderBase {
    public:
        /// Starts the building process for a spotlight.
        SpotLight() {
            spec_.data = specification::SpotLightSpec{};
        }

        /// Sets the RGB color for all lighting components.
        /// @param color The RGB color to apply.
        /// @return Reference to this builder for method chaining.
        auto& color(const glm::vec3& color) {
            return setColor<specification::SpotLightSpec, SpotLight>(color);
        }

        /// Sets the intensity for all lighting components.
        /// @param intensity Intensity value between 0.0 and 1.0.
        /// @return Reference to this builder for method chaining.
        auto& intensity(const float intensity) {
            return setIntensity<specification::SpotLightSpec, SpotLight>(intensity);
        }

        /// Sets color and intensity of the ambient component.
        /// @param color The RGB color to apply.
        /// @param intensity Intensity value between 0.0 and 1.0.
        /// @return Reference to this builder for method chaining.
        auto& ambient(const glm::vec3& color, const float intensity) {
            return setAmbient<specification::SpotLightSpec, SpotLight>(color, intensity);
        }

        /// Sets color and intensity of the diffuse component.
        /// @param color The RGB color to apply.
        /// @param intensity Intensity value between 0.0 and 1.0.
        /// @return Reference to this builder for method chaining.
        auto& diffuse(const glm::vec3& color, const float intensity) {
            return setDiffuse<specification::SpotLightSpec, SpotLight>(color, intensity);
        }

        /// Sets color and intensity of the specular component.
        /// @param color The RGB color to apply.
        /// @param intensity Intensity value between 0.0 and 1.0.
        /// @return Reference to this builder for method chaining.
        auto& specular(const glm::vec3& color, const float intensity) {
            return setSpecular<specification::SpotLightSpec, SpotLight>(color, intensity);
        }

        /// Sets the position of the spotlight being built.
        /// @param position The position vector for the light.
        /// @return Reference to this builder for method chaining.
        auto& position(const glm::vec3& position) {
            std::get<specification::SpotLightSpec>(spec_.data).position = position;
            return *this;
        }

        /// Sets the direction of the spotlight being built.
        /// @param direction The direction vector for the light.
        /// @return Reference to this builder for method chaining.
        auto& direction(const glm::vec3& direction) {
            std::get<specification::SpotLightSpec>(spec_.data).direction = direction;
            return *this;
        }

        /// Sets the attenuation directly for the spotlight being built.
        /// @param attenuation The attenuation factors (constant, linear, quadratic).
        /// @return Reference to this builder for method chaining.
        auto& attenuation(const scene::lighting::Attenuation& attenuation) {
            auto& data = std::get<specification::SpotLightSpec>(spec_.data);
            data.attenuation = attenuation;
            data.range.reset();
            return *this;
        }

        /// Sets the attenuation based on range for the spotlight being built.
        /// @param range The range of the light.
        /// @return Reference to this builder for method chaining.
        auto& range(const scene::lighting::Range range) {
            auto& data = std::get<specification::SpotLightSpec>(spec_.data);
            data.range = range;
            data.attenuation.reset();
            return *this;
        }

        /// Sets the cutoff angles for the spotlight being built.
        /// @param innerCutoff The inner cutoff angle in degrees.
        /// @param outerCutoff The outer cutoff angle in degrees.
        /// @return Reference to this builder for method chaining.
        auto& cutoffAngles(const float innerCutoff, const float outerCutoff) {
            auto& data = std::get<specification::SpotLightSpec>(spec_.data);
            data.innerCutoff = innerCutoff;
            data.outerCutoff = outerCutoff;
            return *this;
        }
    };

} // namespace pixelforge::builders
