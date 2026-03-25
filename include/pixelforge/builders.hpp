#pragma once

#include <pixelforge/scene/builders/nodes/scene_builder.hpp>
#include <pixelforge/scene/builders/nodes/model_builder.hpp>
#include <pixelforge/scene/builders/nodes/render_unit_builder.hpp>
#include <pixelforge/scene/builders/nodes/light_unit_builder.hpp>

#include <pixelforge/scene/builders/lighting/ambient_light_builder.hpp>
#include <pixelforge/scene/builders/lighting/directional_light_builder.hpp>
#include <pixelforge/scene/builders/lighting/positional_light_builder.hpp>
#include <pixelforge/scene/builders/lighting/spot_light_builder.hpp>

#include <pixelforge/scene/builders/animation/orbiting_builder.hpp>
#include <pixelforge/scene/builders/animation/rotation_builder.hpp>
#include <pixelforge/scene/builders/animation/scaling_builder.hpp>
#include <pixelforge/scene/builders/animation/translation_builder.hpp>

namespace pixelforge::builders {
    using scene::nodes::SceneBuilder;
    using scene::nodes::ModelBuilder;
    using scene::nodes::RenderUnitBuilder;
    using scene::nodes::LightUnitBuilder;

    using scene::lighting::AmbientLightBuilder;
    using scene::lighting::DirectionalLightBuilder;
    using scene::lighting::PositionalLightBuilder;
    using scene::lighting::SpotLightBuilder;

    using scene::animation::OrbitingBuilder;
    using scene::animation::RotationBuilder;
    using scene::animation::ScalingBuilder;
    using scene::animation::TranslationBuilder;
}
