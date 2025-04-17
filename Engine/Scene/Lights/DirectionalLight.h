#pragma once
#include "../SceneNodes/Light.h"
#include <GLM/glm.hpp>
using namespace glm;


class DirectionalLight final : public Light {
public:
    explicit DirectionalLight(const string&, const vec3&);
    void SetDirection(const vec3&);
    [[nodiscard]] vec3 GetCurrentDirection() const override;

private:
    vec3 direction;
};
