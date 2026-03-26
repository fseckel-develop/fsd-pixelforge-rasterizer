#include <doctest.h>
#include <managers/light_manager.hpp>
#include <pixelforge/scene/lighting/ambient_light.hpp>
#include <pixelforge/scene/lighting/directional_light.hpp>
#include <pixelforge/scene/lighting/positional_light.hpp>
#include <pixelforge/scene/lighting/spot_light.hpp>
#include <memory>

using pixelforge::management::LightManager;
using pixelforge::scene::lighting::AmbientLight;
using pixelforge::scene::lighting::DirectionalLight;
using pixelforge::scene::lighting::Light;
using pixelforge::scene::lighting::PositionalLight;
using pixelforge::scene::lighting::SpotLight;


namespace {

    struct LightManagerGuard {
        LightManagerGuard() { LightManager::clear(); }
        ~LightManagerGuard() { LightManager::clear(); }
    };

} // namespace


TEST_CASE("LightManager returns same pointer for same light instance") {
    LightManagerGuard guard;

    const auto light = std::make_shared<DirectionalLight>(glm::vec3(0.0f, -1.0f, 0.0f));

    const auto managedA = LightManager::getOrCreate(light);
    const auto managedB = LightManager::getOrCreate(light);

    CHECK(managedA == light);
    CHECK(managedB == light);
    CHECK(managedA == managedB);
}

TEST_CASE("LightManager canonicalizes equivalent ambient lights") {
    LightManagerGuard guard;

    const auto lightA = std::make_shared<AmbientLight>(glm::vec3(0.2f, 0.4f, 0.6f), 0.8f);
    const auto lightB = std::make_shared<AmbientLight>(glm::vec3(0.2f, 0.4f, 0.6f), 0.8f);

    REQUIRE(lightA != lightB);

    const auto managedA = LightManager::getOrCreate(lightA);
    const auto managedB = LightManager::getOrCreate(lightB);

    CHECK(managedA == managedB);
}

TEST_CASE("LightManager distinguishes ambient lights with different ambient data") {
    LightManagerGuard guard;

    const auto lightA = std::make_shared<AmbientLight>(glm::vec3(0.2f, 0.4f, 0.6f), 0.8f);
    const auto lightB = std::make_shared<AmbientLight>(glm::vec3(0.3f, 0.4f, 0.6f), 0.8f);

    const auto managedA = LightManager::getOrCreate(lightA);
    const auto managedB = LightManager::getOrCreate(lightB);

    CHECK(managedA != managedB);
}

TEST_CASE("LightManager canonicalizes equivalent directional lights") {
    LightManagerGuard guard;

    const auto lightA = std::make_shared<DirectionalLight>(glm::vec3(0.0f, -2.0f, 0.0f));
    const auto lightB = std::make_shared<DirectionalLight>(glm::vec3(0.0f, -1.0f, 0.0f));

    REQUIRE(lightA != lightB);

    const auto managedA = LightManager::getOrCreate(lightA);
    const auto managedB = LightManager::getOrCreate(lightB);

    CHECK(managedA == managedB);
}

TEST_CASE("LightManager distinguishes directional lights with different direction") {
    LightManagerGuard guard;

    const auto lightA = std::make_shared<DirectionalLight>(glm::vec3(1.0f, 0.0f, 0.0f));
    const auto lightB = std::make_shared<DirectionalLight>(glm::vec3(0.0f, 1.0f, 0.0f));

    const auto managedA = LightManager::getOrCreate(lightA);
    const auto managedB = LightManager::getOrCreate(lightB);

    CHECK(managedA != managedB);
}

TEST_CASE("LightManager canonicalizes equivalent positional lights") {
    LightManagerGuard guard;

    const auto lightA = std::make_shared<PositionalLight>(glm::vec3(1.0f, 2.0f, 3.0f));
    const auto lightB = std::make_shared<PositionalLight>(glm::vec3(1.0f, 2.0f, 3.0f));

    REQUIRE(lightA != lightB);

    const auto managedA = LightManager::getOrCreate(lightA);
    const auto managedB = LightManager::getOrCreate(lightB);

    CHECK(managedA == managedB);
}

TEST_CASE("LightManager distinguishes positional lights with different position") {
    LightManagerGuard guard;

    const auto lightA = std::make_shared<PositionalLight>(glm::vec3(1.0f, 2.0f, 3.0f));
    const auto lightB = std::make_shared<PositionalLight>(glm::vec3(4.0f, 5.0f, 6.0f));

    const auto managedA = LightManager::getOrCreate(lightA);
    const auto managedB = LightManager::getOrCreate(lightB);

    CHECK(managedA != managedB);
}

TEST_CASE("LightManager distinguishes positional lights with different attenuation") {
    LightManagerGuard guard;

    const auto lightA = std::make_shared<PositionalLight>(glm::vec3(1.0f, 2.0f, 3.0f));
    const auto lightB = std::make_shared<PositionalLight>(glm::vec3(1.0f, 2.0f, 3.0f));

    lightA->setRange(pixelforge::scene::lighting::R100);
    lightB->setRange(pixelforge::scene::lighting::R20);

    const auto managedA = LightManager::getOrCreate(lightA);
    const auto managedB = LightManager::getOrCreate(lightB);

    CHECK(managedA != managedB);
}

TEST_CASE("LightManager distinguishes different light types even with similar shared base data") {
    LightManagerGuard guard;

    const auto ambient = std::make_shared<AmbientLight>(glm::vec3(1.0f), 1.0f);
    const auto directional = std::make_shared<DirectionalLight>(glm::vec3(0.0f, -1.0f, 0.0f));

    const auto managedAmbient = LightManager::getOrCreate(ambient);
    const auto managedDirectional = LightManager::getOrCreate(directional);

    CHECK(managedAmbient != managedDirectional);
}

TEST_CASE("LightManager canonicalizes equivalent spot lights") {
    LightManagerGuard guard;

    const auto lightA = std::make_shared<SpotLight>(glm::vec3(1.0f, 2.0f, 3.0f),
                                                    glm::vec3(0.0f, -2.0f, 0.0f));
    const auto lightB = std::make_shared<SpotLight>(glm::vec3(1.0f, 2.0f, 3.0f),
                                                    glm::vec3(0.0f, -1.0f, 0.0f));

    REQUIRE(lightA != lightB);

    const auto managedA = LightManager::getOrCreate(lightA);
    const auto managedB = LightManager::getOrCreate(lightB);

    CHECK(managedA == managedB);
}

TEST_CASE("LightManager should distinguish spot lights with different direction") {
    LightManagerGuard guard;

    const auto lightA = std::make_shared<SpotLight>(glm::vec3(1.0f, 2.0f, 3.0f),
                                                    glm::vec3(1.0f, 0.0f, 0.0f));
    const auto lightB = std::make_shared<SpotLight>(glm::vec3(1.0f, 2.0f, 3.0f),
                                                    glm::vec3(0.0f, 1.0f, 0.0f));

    const auto managedA = LightManager::getOrCreate(lightA);
    const auto managedB = LightManager::getOrCreate(lightB);

    CHECK(managedA != managedB);
}
