#include <doctest.h>
#include <managers/texture_manager.hpp>
#include <pixelforge/graphics/texturing/cube_map.hpp>
#include <GL/glew.h>

using pixelforge::graphics::CubeMap;
using pixelforge::management::TextureManager;


namespace {

    struct TextureManagerGuard {
        TextureManagerGuard() { TextureManager::clear(); }
        ~TextureManagerGuard() { TextureManager::clear(); }
    };

} // namespace


TEST_CASE("CubeMap constructor creates OpenGL cube map object") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    glGetError();

    const CubeMap cubeMap("test");

    CHECK(glGetError() == GL_NO_ERROR);
    CHECK(cubeMap.getID() != 0);
    CHECK(cubeMap.getType() == GL_TEXTURE_CUBE_MAP);
    CHECK(cubeMap.getUnit() == TextureManager::noUnit());
}

TEST_CASE("CubeMap constructor handles missing faces with fallback uploads") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    glGetError();

    const CubeMap cubeMap("missing_cube_map");

    CHECK(glGetError() == GL_NO_ERROR);
    CHECK(cubeMap.getID() != 0);
}

TEST_CASE("CubeMap bindTexture assigns a texture unit and binds cube map") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    CubeMap cubeMap("test");

    const int unitIndex = cubeMap.bindTexture();

    CHECK(unitIndex >= 0);
    CHECK(cubeMap.getUnit() != TextureManager::noUnit());

    GLint boundTexture = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &boundTexture);
    CHECK(boundTexture == static_cast<GLint>(cubeMap.getID()));
}

TEST_CASE("CubeMap bindTexture reuses already assigned texture unit") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    CubeMap cubeMap("test");

    const int firstUnit = cubeMap.bindTexture();
    const GLenum firstEnumUnit = cubeMap.getUnit();

    const int secondUnit = cubeMap.bindTexture();

    CHECK(secondUnit == firstUnit);
    CHECK(cubeMap.getUnit() == firstEnumUnit);
}

TEST_CASE("CubeMap unbindTexture releases texture unit and unbinds cube map") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    CubeMap cubeMap("test");
    static_cast<void>(cubeMap.bindTexture());

    REQUIRE(cubeMap.getUnit() != TextureManager::noUnit());

    cubeMap.unbindTexture();

    CHECK(cubeMap.getUnit() == TextureManager::noUnit());

    GLint boundTexture = -1;
    glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &boundTexture);
    CHECK(boundTexture == 0);
}

TEST_CASE("CubeMap setWrapping for R S T does not produce OpenGL error") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    CubeMap cubeMap("test");

    glGetError();
    cubeMap.setWrapping(GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT);

    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("CubeMap inherited two-axis setWrapping is a no-op and does not error") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    CubeMap cubeMap("test");

    glGetError();
    cubeMap.setWrapping(GL_REPEAT, GL_REPEAT);

    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("CubeMap deleteTexture deletes texture object without OpenGL error") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    CubeMap cubeMap("test");

    glGetError();
    cubeMap.deleteTexture();

    CHECK(glGetError() == GL_NO_ERROR);
    CHECK(cubeMap.getID() == 0);
}

TEST_CASE("CubeMap works with TextureManager texture unit allocation") {
    TextureManagerGuard guard;
    TextureManager::initialize();

    CubeMap cubeMapA("test");
    CubeMap cubeMapB("test");

    const int unitA = cubeMapA.bindTexture();
    const int unitB = cubeMapB.bindTexture();

    CHECK(unitA != unitB);

    cubeMapA.unbindTexture();
    CHECK(cubeMapA.getUnit() == TextureManager::noUnit());

    const int reusedUnit = cubeMapA.bindTexture();
    CHECK(reusedUnit >= 0);

    cubeMapA.unbindTexture();
    cubeMapB.unbindTexture();
}
