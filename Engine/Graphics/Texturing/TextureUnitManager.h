#pragma once
#include <vector>
#include <GL/glew.h>
using namespace std;


class TextureUnitManager final {
public:
    static void Initialize();
    static GLenum GetFreeUnit();
    static void ReleaseUnit(GLenum);
    static GLenum NoUnit();

private:
    struct TextureUnit { GLenum spot; bool isOccupied; };
    static vector<TextureUnit> textureUnits;
    static constexpr GLenum noUnit = static_cast<GLenum>(-1);
};
