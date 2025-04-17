#include "Scale.h"


Scale::Scale(const vec3& scale) {
    this->scaling = scale;
}


Scale::Scale(const float scaleX, const float scaleY, const float scaleZ) {
    this->scaling = vec3(scaleX, scaleY, scaleZ);
}
