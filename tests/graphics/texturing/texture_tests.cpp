#include <doctest.h>
#include <managers/texture_manager.hpp>
#include <pixelforge/graphics/texturing/texture.hpp>
#include <GL/glew.h>
#include <filesystem>
#include <string>

using pixelforge::graphics::Texture;
using pixelforge::management::TextureManager;


namespace {

    struct TextureManagerGuard {
        TextureManagerGuard() { TextureManager::clear(); }
        ~TextureManagerGuard() { TextureManager::clear(); }
    };

} // namespace


TEST_CASE("Texture constructor creates OpenGL texture object even when file is missing") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    glGetError();

    const Texture texture("missing.jpg");

    CHECK(glGetError() == GL_NO_ERROR);
    CHECK(texture.getID() != 0);
    CHECK(texture.getType() == GL_TEXTURE_2D);
    CHECK(texture.getUnit() == TextureManager::noUnit());
}

TEST_CASE("Texture constructor stores texture path suffix and default type") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    const Texture texture("test.jpg");

    CHECK(texture.getType() == GL_TEXTURE_2D);
    CHECK(texture.getFilePath().find("test.jpg") != std::string::npos);
}

TEST_CASE("Texture bindTexture assigns a texture unit and binds texture") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    Texture texture("test.jpg");

    GLint activeTexture = 0;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTexture);

    const int unitIndex = texture.bindTexture();

    CHECK(unitIndex >= 0);
    CHECK(texture.getUnit() != TextureManager::noUnit());

    GLint boundTexture = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);
    CHECK(boundTexture == static_cast<GLint>(texture.getID()));
}

TEST_CASE("Texture bindTexture reuses already assigned texture unit") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    Texture texture("test.jpg");

    const int firstUnit = texture.bindTexture();
    const GLenum firstEnumUnit = texture.getUnit();

    const int secondUnit = texture.bindTexture();

    CHECK(secondUnit == firstUnit);
    CHECK(texture.getUnit() == firstEnumUnit);
}

TEST_CASE("Texture unbindTexture releases texture unit and unbinds texture") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    Texture texture("test.jpg");
    static_cast<void>(texture.bindTexture());

    REQUIRE(texture.getUnit() != TextureManager::noUnit());

    texture.unbindTexture();

    CHECK(texture.getUnit() == TextureManager::noUnit());

    GLint boundTexture = -1;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);
    CHECK(boundTexture == 0);
}

TEST_CASE("Texture setWrapping does not produce OpenGL error") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    const Texture texture("test.jpg");

    glGetError();
    texture.setWrapping(GL_REPEAT, GL_CLAMP_TO_EDGE);

    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("Texture setFiltering does not produce OpenGL error") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    const Texture texture("test.jpg");

    glGetError();
    texture.setFiltering(GL_LINEAR, GL_LINEAR);

    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("Texture deleteTexture deletes texture object without OpenGL error") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    Texture texture("test.jpg");

    glGetError();
    texture.deleteTexture();

    CHECK(glGetError() == GL_NO_ERROR);
    CHECK(texture.getID() == 0);
}

TEST_CASE("Texture bind and unbind cooperate with TextureManager units") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    Texture textureA("missing_a.jpg");
    Texture textureB("missing_b.jpg");

    const int unitA = textureA.bindTexture();
    const int unitB = textureB.bindTexture();

    CHECK(unitA != unitB);

    textureA.unbindTexture();
    CHECK(textureA.getUnit() == TextureManager::noUnit());

    const int unitAReused = textureA.bindTexture();
    CHECK(unitAReused >= 0);

    textureA.unbindTexture();
    textureB.unbindTexture();
}
