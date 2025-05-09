#include "Translation.h"
using namespace glm;


Translation::Translation(const vec3& translation) {
    this->translation.SetTranslation(translation);
}


Translation::Translation(const float translateX, const float translateY, const float translateZ) {
    this->translation.SetTranslation(translateX, translateY, translateZ);
}
