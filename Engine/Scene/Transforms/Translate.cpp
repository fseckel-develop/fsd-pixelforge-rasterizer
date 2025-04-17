#include "Translate.h"


Translate::Translate(const vec3& translation) {
    this->translation = translation;
}


Translate::Translate(const float translateX, const float translateY, const float translateZ) {
    this->translation = vec3(translateX, translateY, translateZ);
}
