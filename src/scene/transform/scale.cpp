#include <pixelforge/scene/transform/scale.hpp>


namespace pixelforge::scene::transform {

    using glm::vec3;


    Scale::Scale(const float scale) {
        this->scale_.setScale(scale);
    }


    Scale::Scale(const vec3& scale) {
        this->scale_.setScale(scale);
    }


    Scale::Scale(const float scaleX, const float scaleY, const float scaleZ) {
        this->scale_.setScale(scaleX, scaleY, scaleZ);
    }

} // namespace pixelforge::scene::transform
