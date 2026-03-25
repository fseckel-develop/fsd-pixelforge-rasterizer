#include "builders/compiler/light_compiler.hpp"
#include <pixelforge/scene/lighting/ambient_light.hpp>
#include <pixelforge/scene/lighting/directional_light.hpp>
#include <pixelforge/scene/lighting/positional_light.hpp>
#include <pixelforge/scene/lighting/spot_light.hpp>
#include <variant>


namespace pixelforge::scene::builders::compiler {

    template<class>
    inline constexpr bool always_false_v = false;

    std::shared_ptr<lighting::Light> buildLight(
        const pixelforge::builders::specification::LightSpec& spec)
    {
        return std::visit([]<typename dataT>(const dataT& data) -> std::shared_ptr<lighting::Light> {
            using T = std::decay_t<dataT>;

            if constexpr (std::is_same_v<T, pixelforge::builders::specification::AmbientLightSpec>) {
                auto light = std::make_shared<lighting::AmbientLight>();
                if (data.ambient) light->setAmbient(data.ambient->color, data.ambient->intensity);
                return light;
            }
            else if constexpr (std::is_same_v<T, pixelforge::builders::specification::DirectionalLightSpec>) {
                auto light = std::make_shared<lighting::DirectionalLight>();
                if (data.ambient) light->setAmbient(data.ambient->color, data.ambient->intensity);
                if (data.diffuse) light->setDiffuse(data.diffuse->color, data.diffuse->intensity);
                if (data.specular) light->setSpecular(data.specular->color, data.specular->intensity);
                if (data.direction) light->setDirection(*data.direction);
                return light;
            }
            else if constexpr (std::is_same_v<T, pixelforge::builders::specification::PositionalLightSpec>) {
                auto light = std::make_shared<lighting::PositionalLight>();
                if (data.ambient) light->setAmbient(data.ambient->color, data.ambient->intensity);
                if (data.diffuse) light->setDiffuse(data.diffuse->color, data.diffuse->intensity);
                if (data.specular) light->setSpecular(data.specular->color, data.specular->intensity);
                if (data.position) light->setPosition(*data.position);
                if (data.range) light->setRange(*data.range);
                else if (data.attenuation) light->setAttenuation(*data.attenuation);
                return light;
            }
            else if constexpr (std::is_same_v<T, pixelforge::builders::specification::SpotLightSpec>) {
                auto light = std::make_shared<lighting::SpotLight>();
                if (data.ambient) light->setAmbient(data.ambient->color, data.ambient->intensity);
                if (data.diffuse) light->setDiffuse(data.diffuse->color, data.diffuse->intensity);
                if (data.specular) light->setSpecular(data.specular->color, data.specular->intensity);
                if (data.position) light->setPosition(*data.position);
                if (data.direction) light->setDirection(*data.direction);
                if (data.range) light->setRange(*data.range);
                else if (data.attenuation) light->setAttenuation(*data.attenuation);
                if (data.innerCutoff && data.outerCutoff) light->setCutoffAngles(*data.innerCutoff, *data.outerCutoff);
                return light;
            }
            else static_assert(always_false_v<T>, "Unhandled light spec type");

        }, spec.data);
    }

} // namespace pixelforge::scene::builders::compiler
