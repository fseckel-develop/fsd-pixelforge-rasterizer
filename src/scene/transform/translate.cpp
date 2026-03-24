#include <pixelforge/scene/transform/translate.hpp>


namespace pixelforge::scene::transform {

    using glm::vec3;


    Translate::Translate(const vec3& translation) {
        this->translation_.setTranslation(translation);
    }


    Translate::Translate(const float translateX, const float translateY, const float translateZ) {
        this->translation_.setTranslation(translateX, translateY, translateZ);
    }

} // namespace pixelforge::scene::transform
