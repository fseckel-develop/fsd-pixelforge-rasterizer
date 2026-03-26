#include <doctest.h>
#include <managers/material_manager.hpp>
#include <pixelforge/graphics/texturing/material.hpp>
#include <GLM/glm.hpp>
#include <memory>

using pixelforge::graphics::Material;
using pixelforge::management::MaterialManager;


namespace {

    struct MaterialManagerGuard {
        MaterialManagerGuard() { MaterialManager::clear(); }
        ~MaterialManagerGuard() { MaterialManager::clear(); }
    };

} // namespace


TEST_CASE("MaterialManager returns same pointer for same material instance") {
    MaterialManagerGuard guard;

    const auto material = std::make_shared<Material>();
    material->setAmbientColor(glm::vec4(0.1f, 0.2f, 0.3f, 1.0f));
    material->setDiffuseColor(glm::vec4(0.4f, 0.5f, 0.6f, 1.0f));
    material->setSpecularColor(glm::vec4(0.7f, 0.8f, 0.9f, 1.0f));
    material->setShininess(32.0f);

    const auto managedA = MaterialManager::getOrCreate(material);
    const auto managedB = MaterialManager::getOrCreate(material);

    CHECK(managedA == material);
    CHECK(managedB == material);
    CHECK(managedA == managedB);
}

TEST_CASE("MaterialManager canonicalizes equivalent plain materials") {
    MaterialManagerGuard guard;

    const auto materialA = std::make_shared<Material>();
    materialA->setAmbientColor(glm::vec4(0.1f, 0.2f, 0.3f, 1.0f));
    materialA->setDiffuseColor(glm::vec4(0.4f, 0.5f, 0.6f, 1.0f));
    materialA->setSpecularColor(glm::vec4(0.7f, 0.8f, 0.9f, 1.0f));
    materialA->setShininess(32.0f);

    const auto materialB = std::make_shared<Material>();
    materialB->setAmbientColor(glm::vec4(0.1f, 0.2f, 0.3f, 1.0f));
    materialB->setDiffuseColor(glm::vec4(0.4f, 0.5f, 0.6f, 1.0f));
    materialB->setSpecularColor(glm::vec4(0.7f, 0.8f, 0.9f, 1.0f));
    materialB->setShininess(32.0f);

    REQUIRE(materialA != materialB);

    const auto managedA = MaterialManager::getOrCreate(materialA);
    const auto managedB = MaterialManager::getOrCreate(materialB);

    CHECK(managedA == managedB);
}

TEST_CASE("MaterialManager distinguishes materials with different ambient color") {
    MaterialManagerGuard guard;

    const auto materialA = std::make_shared<Material>();
    materialA->setAmbientColor(glm::vec4(0.1f, 0.2f, 0.3f, 1.0f));

    const auto materialB = std::make_shared<Material>();
    materialB->setAmbientColor(glm::vec4(0.9f, 0.2f, 0.3f, 1.0f));

    const auto managedA = MaterialManager::getOrCreate(materialA);
    const auto managedB = MaterialManager::getOrCreate(materialB);

    CHECK(managedA != managedB);
}

TEST_CASE("MaterialManager distinguishes materials with different diffuse color") {
    MaterialManagerGuard guard;

    const auto materialA = std::make_shared<Material>();
    materialA->setDiffuseColor(glm::vec4(0.1f, 0.2f, 0.3f, 1.0f));

    const auto materialB = std::make_shared<Material>();
    materialB->setDiffuseColor(glm::vec4(0.1f, 0.9f, 0.3f, 1.0f));

    const auto managedA = MaterialManager::getOrCreate(materialA);
    const auto managedB = MaterialManager::getOrCreate(materialB);

    CHECK(managedA != managedB);
}

TEST_CASE("MaterialManager distinguishes materials with different specular color") {
    MaterialManagerGuard guard;

    const auto materialA = std::make_shared<Material>();
    materialA->setSpecularColor(glm::vec4(0.1f, 0.2f, 0.3f, 1.0f));

    const auto materialB = std::make_shared<Material>();
    materialB->setSpecularColor(glm::vec4(0.1f, 0.2f, 0.9f, 1.0f));

    const auto managedA = MaterialManager::getOrCreate(materialA);
    const auto managedB = MaterialManager::getOrCreate(materialB);

    CHECK(managedA != managedB);
}

TEST_CASE("MaterialManager distinguishes materials with different shininess") {
    MaterialManagerGuard guard;

    const auto materialA = std::make_shared<Material>();
    materialA->setShininess(16.0f);

    const auto materialB = std::make_shared<Material>();
    materialB->setShininess(64.0f);

    const auto managedA = MaterialManager::getOrCreate(materialA);
    const auto managedB = MaterialManager::getOrCreate(materialB);

    CHECK(managedA != managedB);
}

TEST_CASE("MaterialManager canonicalizes built-in materials with same values") {
    MaterialManagerGuard guard;

    const auto materialA = std::make_shared<pixelforge::graphics::Gold>();
    const auto materialB = std::make_shared<pixelforge::graphics::Gold>();

    const auto managedA = MaterialManager::getOrCreate(materialA);
    const auto managedB = MaterialManager::getOrCreate(materialB);

    CHECK(managedA == managedB);
}

TEST_CASE("MaterialManager distinguishes different built-in material presets") {
    MaterialManagerGuard guard;

    const auto gold = std::make_shared<pixelforge::graphics::Gold>();
    const auto silver = std::make_shared<pixelforge::graphics::Silver>();

    const auto managedGold = MaterialManager::getOrCreate(gold);
    const auto managedSilver = MaterialManager::getOrCreate(silver);

    CHECK(managedGold != managedSilver);
}
