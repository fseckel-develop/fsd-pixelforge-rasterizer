#include <doctest.h>
#include <managers/texture_manager.hpp>
#include <pixelforge/graphics/texturing/texture.hpp>
#include <GL/glew.h>
#include <memory>
#include <vector>

using pixelforge::graphics::Texture;
using pixelforge::management::TextureManager;


namespace {

    struct TextureManagerGuard {
        TextureManagerGuard() {
            TextureManager::clear();
        }

        ~TextureManagerGuard() {
            TextureManager::clear();
        }
    };

} // namespace


TEST_CASE("TextureManager initialize prepares at least one texture unit") {
    TextureManagerGuard guard;

    TextureManager::initialize();

    GLenum unit = TextureManager::getFreeUnit();

    CHECK(unit >= GL_TEXTURE0);

    TextureManager::releaseUnit(unit);
    CHECK(unit == TextureManager::noUnit());
}

TEST_CASE("TextureManager noUnit returns sentinel value") {
    TextureManagerGuard guard;

    CHECK(TextureManager::noUnit() == static_cast<GLenum>(-1));
}

TEST_CASE("TextureManager getFreeUnit returns distinct units until released") {
    TextureManagerGuard guard;

    TextureManager::initialize();

    GLenum unitA = TextureManager::getFreeUnit();
    GLenum unitB = TextureManager::getFreeUnit();

    CHECK(unitA != unitB);
    CHECK(unitA >= GL_TEXTURE0);
    CHECK(unitB >= GL_TEXTURE0);

    TextureManager::releaseUnit(unitA);
    TextureManager::releaseUnit(unitB);
}

TEST_CASE("TextureManager releaseUnit resets unit to noUnit") {
    TextureManagerGuard guard;

    TextureManager::initialize();

    GLenum unit = TextureManager::getFreeUnit();
    REQUIRE(unit != TextureManager::noUnit());

    TextureManager::releaseUnit(unit);

    CHECK(unit == TextureManager::noUnit());
}

TEST_CASE("TextureManager released unit becomes reusable") {
    TextureManagerGuard guard;

    TextureManager::initialize();

    GLenum unitA = TextureManager::getFreeUnit();
    REQUIRE(unitA != TextureManager::noUnit());

    TextureManager::releaseUnit(unitA);
    REQUIRE(unitA == TextureManager::noUnit());

    GLenum unitB = TextureManager::getFreeUnit();

    CHECK(unitB >= GL_TEXTURE0);
}

TEST_CASE("TextureManager getFreeUnit throws when all units are occupied") {
    TextureManagerGuard guard;

    TextureManager::initialize();

    GLint maxUnits = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnits);
    REQUIRE(maxUnits > 0);

    std::vector<GLenum> allocated;
    allocated.reserve(static_cast<std::size_t>(maxUnits));

    for (GLint i = 0; i < maxUnits; ++i) {
        allocated.push_back(TextureManager::getFreeUnit());
    }

    CHECK_THROWS_AS(TextureManager::getFreeUnit(), std::runtime_error);

    for (auto& unit : allocated) {
        TextureManager::releaseUnit(unit);
    }
}

TEST_CASE("TextureManager releaseUnit ignores unknown unit values") {
    TextureManagerGuard guard;

    TextureManager::initialize();

    GLenum bogus = 123456;
    TextureManager::releaseUnit(bogus);

    CHECK(bogus == static_cast<GLenum>(123456));
}

TEST_CASE("TextureManager returns same pointer for same texture instance") {
    TextureManagerGuard guard;

    const auto texture = std::make_shared<Texture>("test.jpg");

    const auto managedA = TextureManager::getOrCreate(texture);
    const auto managedB = TextureManager::getOrCreate(texture);

    CHECK(managedA == texture);
    CHECK(managedB == texture);
    CHECK(managedA == managedB);
}

TEST_CASE("TextureManager canonicalizes equivalent textures") {
    TextureManagerGuard guard;

    const auto textureA = std::make_shared<Texture>("test.jpg");
    const auto textureB = std::make_shared<Texture>("test.jpg");

    const auto managedA = TextureManager::getOrCreate(textureA);
    const auto managedB = TextureManager::getOrCreate(textureB);

    CHECK(managedA == managedB);
}

TEST_CASE("TextureManager distinguishes textures by file path") {
    TextureManagerGuard guard;

    const auto textureA = std::make_shared<Texture>("test.jpg");
    const auto textureB = std::make_shared<Texture>("earth.jpg");

    const auto managedA = TextureManager::getOrCreate(textureA);
    const auto managedB = TextureManager::getOrCreate(textureB);

    CHECK(managedA != managedB);
}
