#include "Rotation.h"


Rotation::Rotation(const quat& rotation) {
    this->rotation.SetRotation(rotation);
}


Rotation::Rotation(const vec3& eulerAngles) {
    this->rotation.SetRotation(eulerAngles);
}


Rotation::Rotation(const float pitch, const float yaw, const float roll) {
    this->rotation.SetRotation(pitch, yaw, roll);
}
