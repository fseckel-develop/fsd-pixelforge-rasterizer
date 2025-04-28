#pragma once


template<typename LightT, typename DerivedBuilder> class LightBuilder {
public:
    LightBuilder() {
        light = make_shared<LightT>();
    }

    auto& withColor(const vec3& color) {
        light->SetColor(color);
        return static_cast<DerivedBuilder&>(*this);
    }

    auto& withIntensity(const float intensity) {
        light->SetIntensity(intensity);
        return static_cast<DerivedBuilder&>(*this);
    }

    auto& withAmbient(const vec3& color, const float intensity) {
        light->SetAmbient(color, intensity);
        return static_cast<DerivedBuilder&>(*this);
    }

    auto& withDiffuse(const vec3& color, const float intensity) {
        light->SetDiffuse(color, intensity);
        return static_cast<DerivedBuilder&>(*this);
    }

    auto& withSpecular(const vec3& color, const float intensity) {
        light->SetSpecular(color, intensity);
        return static_cast<DerivedBuilder&>(*this);
    }

    shared_ptr<LightT> Build() const {
        return light;
    }

    operator shared_ptr<LightT>() { // NOLINT
        return Build();
    }

protected:
    shared_ptr<LightT> light;
};
