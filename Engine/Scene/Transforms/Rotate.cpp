#include "Rotate.h"


Rotate::Rotate(const quat& rotation) {
    this->rotation = rotation;
}


Rotate::Rotate(const vec3& eulerAngles) {
    this->rotation = quat(eulerAngles);
}
