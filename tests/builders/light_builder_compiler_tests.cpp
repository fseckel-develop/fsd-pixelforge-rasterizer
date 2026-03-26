#include <doctest.h>
#include <pixelforge/builders/light.hpp>
#include <pixelforge/builders/specification/light_spec.hpp>
#include <builders/compiler/light_compiler.hpp>
#include <pixelforge/scene/lighting/ambient_light.hpp>
#include <pixelforge/scene/lighting/directional_light.hpp>
#include <pixelforge/scene/lighting/positional_light.hpp>
#include <pixelforge/scene/lighting/spot_light.hpp>
#include <GLM/glm.hpp>
#include <memory>
#include <variant>

using pixelforge::builders::AmbientLight;
using pixelforge::builders::DirectionalLight;
using pixelforge::builders::PositionalLight;
using pixelforge::builders::SpotLight;
using pixelforge::builders::specification::AmbientLightSpec;
using pixelforge::builders::specification::DirectionalLightSpec;
using pixelforge::builders::specification::LightSpec;
using pixelforge::builders::specification::PositionalLightSpec;
using pixelforge::builders::specification::SpotLightSpec;
using pixelforge::scene::builders::compiler::buildLight;
using pixelforge::scene::lighting::Attenuation;
using pixelforge::scene::lighting::Light;
using pixelforge::scene::lighting::Range;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected, float eps = EPS) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(eps));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(eps));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(eps));
    }

    void checkComponent(const pixelforge::scene::lighting::LightComponent& actual,
                        const glm::vec3& color,
                        const float intensity,
                        const float eps = EPS) {
        checkVec3Approx(actual.color, color, eps);
        CHECK(actual.intensity == doctest::Approx(intensity).epsilon(eps));
    }

    void checkAttenuation(const Attenuation& actual,
                          const Attenuation& expected,
                          const float eps = EPS) {
        CHECK(actual.constant == doctest::Approx(expected.constant).epsilon(eps));
        CHECK(actual.linear == doctest::Approx(expected.linear).epsilon(eps));
        CHECK(actual.quadratic == doctest::Approx(expected.quadratic).epsilon(eps));
    }

} // namespace


TEST_CASE("AmbientLight builder builds ambient light spec with configured values") {
    const LightSpec spec = AmbientLight()
        .color(glm::vec3(0.2f, 0.4f, 0.6f))
        .intensity(0.8f)
        .buildSpec();

    REQUIRE(std::holds_alternative<AmbientLightSpec>(spec.data));
    const auto& data = std::get<AmbientLightSpec>(spec.data);

    REQUIRE(data.ambient.has_value());
    checkComponent(*data.ambient, glm::vec3(0.2f, 0.4f, 0.6f), 0.8f);
}

TEST_CASE("DirectionalLight builder builds directional light spec with configured values") {
    const LightSpec spec = DirectionalLight()
        .color(glm::vec3(0.3f, 0.5f, 0.7f))
        .intensity(0.6f)
        .ambient(glm::vec3(0.1f, 0.2f, 0.3f), 0.4f)
        .diffuse(glm::vec3(0.4f, 0.5f, 0.6f), 0.7f)
        .specular(glm::vec3(0.7f, 0.8f, 0.9f), 1.0f)
        .direction(glm::vec3(0.0f, -1.0f, 0.0f))
        .buildSpec();

    REQUIRE(std::holds_alternative<DirectionalLightSpec>(spec.data));
    const auto& data = std::get<DirectionalLightSpec>(spec.data);

    REQUIRE(data.ambient.has_value());
    REQUIRE(data.diffuse.has_value());
    REQUIRE(data.specular.has_value());
    REQUIRE(data.direction.has_value());

    checkComponent(*data.ambient, glm::vec3(0.1f, 0.2f, 0.3f), 0.4f);
    checkComponent(*data.diffuse, glm::vec3(0.4f, 0.5f, 0.6f), 0.7f);
    checkComponent(*data.specular, glm::vec3(0.7f, 0.8f, 0.9f), 1.0f);
    checkVec3Approx(*data.direction, glm::vec3(0.0f, -1.0f, 0.0f));
}

TEST_CASE("PositionalLight builder stores range and clears explicit attenuation") {
    const LightSpec spec = PositionalLight()
        .attenuation(Attenuation{1.0f, 0.2f, 0.3f})
        .range(Range::R50)
        .buildSpec();

    REQUIRE(std::holds_alternative<PositionalLightSpec>(spec.data));
    const auto& data = std::get<PositionalLightSpec>(spec.data);

    REQUIRE(data.range.has_value());
    CHECK(data.range == Range::R50);
    CHECK_FALSE(data.attenuation.has_value());
}

TEST_CASE("PositionalLight builder stores explicit attenuation and clears range") {
    const LightSpec spec = PositionalLight()
        .range(Range::R100)
        .attenuation(Attenuation{1.0f, 0.2f, 0.3f})
        .buildSpec();

    REQUIRE(std::holds_alternative<PositionalLightSpec>(spec.data));
    const auto& data = std::get<PositionalLightSpec>(spec.data);

    REQUIRE(data.attenuation.has_value());
    checkAttenuation(*data.attenuation, Attenuation{1.0f, 0.2f, 0.3f});
    CHECK_FALSE(data.range.has_value());
}

TEST_CASE("SpotLight builder stores cutoff angles and lighting properties") {
    const LightSpec spec = SpotLight()
        .color(glm::vec3(0.2f, 0.3f, 0.4f))
        .intensity(0.5f)
        .position(glm::vec3(1.0f, 2.0f, 3.0f))
        .direction(glm::vec3(0.0f, -1.0f, 0.0f))
        .cutoffAngles(10.0f, 20.0f)
        .buildSpec();

    REQUIRE(std::holds_alternative<SpotLightSpec>(spec.data));
    const auto& data = std::get<SpotLightSpec>(spec.data);

    REQUIRE(data.ambient.has_value());
    REQUIRE(data.diffuse.has_value());
    REQUIRE(data.specular.has_value());
    REQUIRE(data.position.has_value());
    REQUIRE(data.direction.has_value());
    REQUIRE(data.innerCutoff.has_value());
    REQUIRE(data.outerCutoff.has_value());

    checkComponent(*data.ambient, glm::vec3(0.2f, 0.3f, 0.4f), 0.5f);
    checkComponent(*data.diffuse, glm::vec3(0.2f, 0.3f, 0.4f), 0.5f);
    checkComponent(*data.specular, glm::vec3(0.2f, 0.3f, 0.4f), 0.5f);
    checkVec3Approx(*data.position, glm::vec3(1.0f, 2.0f, 3.0f));
    checkVec3Approx(*data.direction, glm::vec3(0.0f, -1.0f, 0.0f));
    CHECK(*data.innerCutoff == doctest::Approx(10.0f));
    CHECK(*data.outerCutoff == doctest::Approx(20.0f));
}

TEST_CASE("Light compiler builds AmbientLight from spec") {
    const auto spec = AmbientLight()
        .ambient(glm::vec3(0.2f, 0.4f, 0.6f), 0.8f)
        .buildSpec();

    const auto light = buildLight(spec);
    REQUIRE(light != nullptr);

    const auto ambient = std::dynamic_pointer_cast<pixelforge::scene::lighting::AmbientLight>(light);
    REQUIRE(ambient != nullptr);

    CHECK(ambient->getType() == Light::AMBIENT);
    checkComponent(ambient->getAmbient(), glm::vec3(0.2f, 0.4f, 0.6f), 0.8f);
    checkComponent(ambient->getDiffuse(), glm::vec3(0.0f), 0.0f);
    checkComponent(ambient->getSpecular(), glm::vec3(0.0f), 0.0f);
}

TEST_CASE("Light compiler builds DirectionalLight from spec") {
    const auto spec = DirectionalLight()
        .ambient(glm::vec3(0.1f, 0.2f, 0.3f), 0.4f)
        .diffuse(glm::vec3(0.4f, 0.5f, 0.6f), 0.7f)
        .specular(glm::vec3(0.7f, 0.8f, 0.9f), 1.0f)
        .direction(glm::vec3(0.0f, -2.0f, 0.0f))
        .buildSpec();

    const auto light = buildLight(spec);
    REQUIRE(light != nullptr);

    const auto directional = std::dynamic_pointer_cast<pixelforge::scene::lighting::DirectionalLight>(light);
    REQUIRE(directional != nullptr);

    CHECK(directional->getType() == Light::DIRECTIONAL);
    checkComponent(directional->getAmbient(), glm::vec3(0.1f, 0.2f, 0.3f), 0.4f);
    checkComponent(directional->getDiffuse(), glm::vec3(0.4f, 0.5f, 0.6f), 0.7f);
    checkComponent(directional->getSpecular(), glm::vec3(0.7f, 0.8f, 0.9f), 1.0f);
    checkVec3Approx(directional->getDirection(), glm::vec3(0.0f, -1.0f, 0.0f));
}

TEST_CASE("Light compiler builds PositionalLight from spec with explicit attenuation") {
    const auto spec = PositionalLight()
        .ambient(glm::vec3(0.1f, 0.2f, 0.3f), 0.4f)
        .diffuse(glm::vec3(0.4f, 0.5f, 0.6f), 0.7f)
        .specular(glm::vec3(0.7f, 0.8f, 0.9f), 1.0f)
        .position(glm::vec3(1.0f, 2.0f, 3.0f))
        .attenuation(Attenuation{1.0f, 0.2f, 0.3f})
        .buildSpec();

    const auto light = buildLight(spec);
    REQUIRE(light != nullptr);

    const auto positional = std::dynamic_pointer_cast<pixelforge::scene::lighting::PositionalLight>(light);
    REQUIRE(positional != nullptr);

    CHECK(positional->getType() == Light::POSITIONAL);
    checkComponent(positional->getAmbient(), glm::vec3(0.1f, 0.2f, 0.3f), 0.4f);
    checkComponent(positional->getDiffuse(), glm::vec3(0.4f, 0.5f, 0.6f), 0.7f);
    checkComponent(positional->getSpecular(), glm::vec3(0.7f, 0.8f, 0.9f), 1.0f);
    checkVec3Approx(positional->getPosition(), glm::vec3(1.0f, 2.0f, 3.0f));
    checkAttenuation(positional->getAttenuation(), Attenuation{1.0f, 0.2f, 0.3f});
}

TEST_CASE("Light compiler builds PositionalLight from spec with range") {
    const auto spec = PositionalLight()
        .range(Range::R20)
        .buildSpec();

    const auto light = buildLight(spec);
    REQUIRE(light != nullptr);

    const auto positional = std::dynamic_pointer_cast<pixelforge::scene::lighting::PositionalLight>(light);
    REQUIRE(positional != nullptr);

    const auto attenuation = positional->getAttenuation();
    CHECK(attenuation.constant == doctest::Approx(1.0f));
    CHECK(attenuation.linear == doctest::Approx(0.22f));
    CHECK(attenuation.quadratic == doctest::Approx(0.20f));
}

TEST_CASE("Light compiler builds SpotLight from spec") {
    const auto spec = SpotLight()
        .ambient(glm::vec3(0.1f, 0.2f, 0.3f), 0.4f)
        .diffuse(glm::vec3(0.4f, 0.5f, 0.6f), 0.7f)
        .specular(glm::vec3(0.7f, 0.8f, 0.9f), 1.0f)
        .position(glm::vec3(1.0f, 2.0f, 3.0f))
        .direction(glm::vec3(0.0f, -2.0f, 0.0f))
        .attenuation(Attenuation{1.0f, 0.2f, 0.3f})
        .cutoffAngles(10.0f, 20.0f)
        .buildSpec();

    const auto light = buildLight(spec);
    REQUIRE(light != nullptr);

    const auto spot = std::dynamic_pointer_cast<pixelforge::scene::lighting::SpotLight>(light);
    REQUIRE(spot != nullptr);

    CHECK(spot->getType() == Light::SPOT);
    checkComponent(spot->getAmbient(), glm::vec3(0.1f, 0.2f, 0.3f), 0.4f);
    checkComponent(spot->getDiffuse(), glm::vec3(0.4f, 0.5f, 0.6f), 0.7f);
    checkComponent(spot->getSpecular(), glm::vec3(0.7f, 0.8f, 0.9f), 1.0f);
    checkVec3Approx(spot->getPosition(), glm::vec3(1.0f, 2.0f, 3.0f));
    checkVec3Approx(spot->getDirection(), glm::vec3(0.0f, -1.0f, 0.0f));
    checkAttenuation(spot->getAttenuation(), Attenuation{1.0f, 0.2f, 0.3f});
    CHECK(spot->getInnerCutoff() == doctest::Approx(std::cos(glm::radians(10.0f))).epsilon(EPS));
    CHECK(spot->getOuterCutoff() == doctest::Approx(std::cos(glm::radians(20.0f))).epsilon(EPS));
}

TEST_CASE("Light compiler uses runtime defaults when builder leaves fields unspecified") {
    SUBCASE("ambient light defaults") {
        const auto light = buildLight(AmbientLight().buildSpec());
        const auto ambient = std::dynamic_pointer_cast<pixelforge::scene::lighting::AmbientLight>(light);
        REQUIRE(ambient != nullptr);

        CHECK(ambient->getType() == Light::AMBIENT);
        checkComponent(ambient->getAmbient(), glm::vec3(1.0f), 1.0f);
        checkComponent(ambient->getDiffuse(), glm::vec3(0.0f), 0.0f);
        checkComponent(ambient->getSpecular(), glm::vec3(0.0f), 0.0f);
    }

    SUBCASE("directional light defaults") {
        const auto light = buildLight(DirectionalLight().buildSpec());
        const auto directional = std::dynamic_pointer_cast<pixelforge::scene::lighting::DirectionalLight>(light);
        REQUIRE(directional != nullptr);

        CHECK(directional->getType() == Light::DIRECTIONAL);
        checkComponent(directional->getAmbient(), glm::vec3(1.0f), 1.0f);
        checkComponent(directional->getDiffuse(), glm::vec3(1.0f), 1.0f);
        checkComponent(directional->getSpecular(), glm::vec3(1.0f), 1.0f);
        checkVec3Approx(directional->getDirection(), glm::vec3(0.0f, -1.0f, 0.0f));
    }

    SUBCASE("positional light defaults") {
        const auto light = buildLight(PositionalLight().buildSpec());
        const auto positional = std::dynamic_pointer_cast<pixelforge::scene::lighting::PositionalLight>(light);
        REQUIRE(positional != nullptr);

        CHECK(positional->getType() == Light::POSITIONAL);
        checkComponent(positional->getAmbient(), glm::vec3(1.0f), 1.0f);
        checkComponent(positional->getDiffuse(), glm::vec3(1.0f), 1.0f);
        checkComponent(positional->getSpecular(), glm::vec3(1.0f), 1.0f);
        checkVec3Approx(positional->getPosition(), glm::vec3(0.0f));
        checkAttenuation(positional->getAttenuation(), Attenuation{1.0f, 0.045f, 0.0075f});
    }

    SUBCASE("spot light defaults") {
        const auto light = buildLight(SpotLight().buildSpec());
        const auto spot = std::dynamic_pointer_cast<pixelforge::scene::lighting::SpotLight>(light);
        REQUIRE(spot != nullptr);

        CHECK(spot->getType() == Light::SPOT);
        checkComponent(spot->getAmbient(), glm::vec3(1.0f), 1.0f);
        checkComponent(spot->getDiffuse(), glm::vec3(1.0f), 1.0f);
        checkComponent(spot->getSpecular(), glm::vec3(1.0f), 1.0f);
        checkVec3Approx(spot->getPosition(), glm::vec3(0.0f));
        checkVec3Approx(spot->getDirection(), glm::vec3(0.0f, -1.0f, 0.0f));
        CHECK(spot->getInnerCutoff() == doctest::Approx(std::cos(glm::radians(5.0f))).epsilon(EPS));
        CHECK(spot->getOuterCutoff() == doctest::Approx(std::cos(glm::radians(15.0f))).epsilon(EPS));
    }
}

TEST_CASE("Light compiler prefers range over attenuation when both are present in spec") {
    pixelforge::builders::specification::PositionalLightSpec data{};
    data.attenuation = Attenuation{1.0f, 0.2f, 0.3f};
    data.range = Range::R50;

    LightSpec spec;
    spec.data = data;

    const auto light = buildLight(spec);
    const auto positional = std::dynamic_pointer_cast<pixelforge::scene::lighting::PositionalLight>(light);
    REQUIRE(positional != nullptr);

    const auto attenuation = positional->getAttenuation();
    CHECK(attenuation.constant == doctest::Approx(1.0f));
    CHECK(attenuation.linear == doctest::Approx(0.09f));
    CHECK(attenuation.quadratic == doctest::Approx(0.032f));
}

TEST_CASE("Light compiler ignores cutoff angles unless both are present") {
    SUBCASE("only inner cutoff set") {
        SpotLightSpec data{};
        data.innerCutoff = 12.0f;

        LightSpec spec;
        spec.data = data;

        const auto light = buildLight(spec);
        const auto spot = std::dynamic_pointer_cast<pixelforge::scene::lighting::SpotLight>(light);
        REQUIRE(spot != nullptr);

        CHECK(spot->getInnerCutoff() == doctest::Approx(std::cos(glm::radians(5.0f))).epsilon(EPS));
        CHECK(spot->getOuterCutoff() == doctest::Approx(std::cos(glm::radians(15.0f))).epsilon(EPS));
    }

    SUBCASE("only outer cutoff set") {
        SpotLightSpec data{};
        data.outerCutoff = 24.0f;

        LightSpec spec;
        spec.data = data;

        const auto light = buildLight(spec);
        const auto spot = std::dynamic_pointer_cast<pixelforge::scene::lighting::SpotLight>(light);
        REQUIRE(spot != nullptr);

        CHECK(spot->getInnerCutoff() == doctest::Approx(std::cos(glm::radians(5.0f))).epsilon(EPS));
        CHECK(spot->getOuterCutoff() == doctest::Approx(std::cos(glm::radians(15.0f))).epsilon(EPS));
    }
}
