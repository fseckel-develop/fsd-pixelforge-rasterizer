#include "Scale.h"


Scale::Scale(const float scale) {
    this->scale.SetScale(scale);
}


Scale::Scale(const vec3& scale) {
    this->scale.SetScale(scale);
}


Scale::Scale(const float scaleX, const float scaleY, const float scaleZ) {
    this->scale.SetScale(scaleX, scaleY, scaleZ);
}
