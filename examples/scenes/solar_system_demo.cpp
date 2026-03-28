#include "solar_system_demo.hpp"
#include <pixelforge/builders.hpp>
#include <pixelforge/geometry.hpp>
#include <pixelforge/scene/animation/animation.hpp>
#include <pixelforge/geometry/meshes/annulus.hpp>
#include <pixelforge/scene/transform/rotate.hpp>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

using pixelforge::builders::AmbientLight;
using pixelforge::builders::EllipticOrbiting;
using pixelforge::builders::LightUnit;
using pixelforge::builders::Model;
using pixelforge::builders::PositionalLight;
using pixelforge::builders::Rotation;
using pixelforge::builders::Scene;
using pixelforge::scene::animation::Animation;
using pixelforge::scene::transform::Rotate;
using pixelforge::geometry::Sphere;
using pixelforge::geometry::Annulus;
using pixelforge::graphics::Texture;
using pixelforge::graphics::Material;


namespace {

    struct CelestialBodyData {
        std::string name;
        std::string textureFile;
        float radiusKm;
        float orbitSemiMajorAxisMillionKm;
        float eccentricity;
        float orbitPeriodDays;
        float spinPeriodDays;
        float axialTiltDegrees;
        bool retrogradeRotation;
        bool isMoon = false;
    };


    const std::unordered_map<std::string, CelestialBodyData> celestialBodies = {
        {"Sun",     {"Sun",     "solar_system/sun.jpg",     696340.0f,    0.0f,    0.0f,     0.0f,   25.0f,   7.25f, false, false}},
        {"Mercury", {"Mercury", "solar_system/mercury.jpg",   2439.7f,   57.9f, 0.2056f,   87.97f,  58.65f,   0.03f, false, false}},
        {"Venus",   {"Venus",   "solar_system/venus.jpg",     6051.8f,  108.2f, 0.0068f,  224.70f, 243.00f, 177.3f,  true,  false}},
        {"Earth",   {"Earth",   "solar_system/earth.jpg",     6371.0f,  149.6f, 0.0167f,  365.25f,   1.00f,  23.44f, false, false}},
        {"Moon",    {"Moon",    "solar_system/moon.jpg",      1737.4f, 0.3844f, 0.0549f,   27.32f,  27.32f,   6.68f, false,  true}},
        {"Mars",    {"Mars",    "solar_system/mars.jpg",      3389.5f,  227.9f, 0.0934f,  687.00f,   1.03f,  25.19f, false, false}},
        {"Jupiter", {"Jupiter", "solar_system/jupiter.jpg",  69911.0f,  778.6f, 0.0489f,  4332.6f,   0.41f,   3.13f, false, false}},
        {"Saturn",  {"Saturn",  "solar_system/saturn.jpg",   58232.0f, 1433.5f, 0.0565f, 10759.0f,   0.45f,  26.73f, false, false}},
        {"Uranus",  {"Uranus",  "solar_system/uranus.jpg",   25362.0f, 2872.5f, 0.0463f, 30688.0f,   0.72f,  97.77f,  true, false}},
        {"Neptune", {"Neptune", "solar_system/neptune.jpg",  24622.0f, 4495.1f, 0.0095f, 60182.0f,   0.67f,  28.32f, false, false}}
    };


    struct DisplayScaler {
        float orbitDaysPerSecond = 10.0f;
        float spinDaysPerSecond = 0.1f;
        float orbitScale = 0.025f;
        float moonOrbitScale = 1.5f;
        float planetScale = 0.00004f;
        float sunScale = 0.0000018f;
        float minBodyRadius = 0.08f;
        float orbitOffset = 3.0f;
    };

    constexpr DisplayScaler display;


    glm::vec3 spinAxisFor(const CelestialBodyData& body) {
        return body.retrogradeRotation
            ? glm::vec3(0.0f, -1.0f, 0.0f)
            : glm::vec3(0.0f,  1.0f, 0.0f);
    }

    float displayedRadius(const CelestialBodyData& body) {
        if (body.name == "Sun") {
            return body.radiusKm * display.sunScale;
        }
        return std::max(display.minBodyRadius, body.radiusKm * display.planetScale);
    }

    float displayedOrbitSemiMajor(const CelestialBodyData& body) {
        if (body.isMoon) {
            return body.orbitSemiMajorAxisMillionKm * display.moonOrbitScale;
        }
        return body.orbitSemiMajorAxisMillionKm * display.orbitScale + display.orbitOffset;
    }

    float displayedOrbitSemiMinor(const CelestialBodyData& body) {
        const float orbitSemiMajor = displayedOrbitSemiMajor(body);
        return orbitSemiMajor * std::sqrt(std::max(0.0f, 1.0f - body.eccentricity * body.eccentricity));
    }

    float orbitDurationFromDays(const CelestialBodyData& body) {
        return body.orbitPeriodDays / display.orbitDaysPerSecond;
    }

    float spinDurationFromDays(const CelestialBodyData& body) {
        return std::clamp(body.spinPeriodDays / display.spinDaysPerSecond, 2.0f, 80.0f);
    }

    template <typename ModelT, typename ConfigureT>
    void addInvisibleAnchor(ModelT& model, const std::string& name, ConfigureT configure) {
        model.renderUnit(name, [&](auto& renderUnit) {
            renderUnit.mesh(Sphere(0.001f));
            configure(renderUnit);
        });
    }

    template <typename ModelT>
    void addTiltAnchor(ModelT& model, const std::string& name, const std::string& parentName, float axialTiltDegrees) {
        addInvisibleAnchor(model, name, [&](auto& renderUnit) {
            renderUnit.parent(parentName);
            renderUnit.transform(Rotate(axialTiltDegrees, 0.0f, 0.0f));
        });
    }


    void addSaturnSystem(Scene& scene) {
        const auto& saturn = celestialBodies.at("Saturn");
        const float semiMajor = displayedOrbitSemiMajor(saturn);
        const float semiMinor = displayedOrbitSemiMinor(saturn);
        const float radius = displayedRadius(saturn);

        const auto ringTexture = std::make_shared<Texture>("solar_system/saturn_ring.png");
        ringTexture->setWrapping(GL_CLAMP_TO_EDGE, GL_REPEAT);
        const auto ringMaterial = std::make_shared<Material>();
        ringMaterial->setDiffuseMap(ringTexture);
        const float ringInnerRadius = radius * 1.4f;
        const float ringOuterRadius = radius * 2.4f;

        scene.model("SaturnSystem", [&](auto& model) {
            addInvisibleAnchor(model, "SaturnOrbitAnchor", [&](auto& renderUnit) {
                renderUnit.animation(
                    EllipticOrbiting(Animation::LOOP)
                        .semiMajorAxis(semiMajor)
                        .semiMinorAxis(semiMinor)
                        .duration(orbitDurationFromDays(saturn))
                        .useFocusOrigin(true)
                );
            });

            addTiltAnchor(model, "SaturnTiltAnchor", "SaturnOrbitAnchor", saturn.axialTiltDegrees);

            model.renderUnit("Saturn", [&](auto& renderUnit) {
                renderUnit.parent("SaturnTiltAnchor");
                renderUnit.mesh(Sphere(radius));
                renderUnit.texture(saturn.textureFile);
                renderUnit.animation(
                    Rotation(Animation::LOOP)
                        .duration(spinDurationFromDays(saturn))
                        .rotationAxis(spinAxisFor(saturn))
                );
            });

            model.renderUnit("SaturnRing", [&](auto& renderUnit) {
                renderUnit.parent("SaturnTiltAnchor");
                renderUnit.mesh(Annulus(ringInnerRadius, ringOuterRadius));
                renderUnit.material(ringMaterial);
            });
        });
    }


    void addEarthMoonSystem(Scene& scene) {
        const auto& earth = celestialBodies.at("Earth");
        const float earthRadius = displayedRadius(earth);
        const float earthSemiMajor = displayedOrbitSemiMajor(earth);
        const float earthSemiMinor = displayedOrbitSemiMinor(earth);

        const auto& moon = celestialBodies.at("Moon");
        const float moonRadius = displayedRadius(moon);
        const float moonSemiMajor = displayedOrbitSemiMajor(moon);
        const float moonSemiMinor = displayedOrbitSemiMinor(moon);

        scene.model("EarthSystem", [&](auto& model) {

            addInvisibleAnchor(model, "EarthOrbitAnchor", [&](auto& renderUnit) {
                renderUnit.animation(
                    EllipticOrbiting(Animation::LOOP)
                        .semiMajorAxis(earthSemiMajor)
                        .semiMinorAxis(earthSemiMinor)
                        .duration(orbitDurationFromDays(earth))
                        .useFocusOrigin(true)
                );
            });

            addTiltAnchor(model, "EarthTiltAnchor", "EarthOrbitAnchor", earth.axialTiltDegrees);

            model.renderUnit("Earth", [&](auto& renderUnit) {
                renderUnit.parent("EarthTiltAnchor");
                renderUnit.mesh(Sphere(earthRadius));
                renderUnit.texture(earth.textureFile);
                renderUnit.animation(
                    Rotation(Animation::LOOP)
                        .duration(spinDurationFromDays(earth))
                        .rotationAxis(spinAxisFor(earth))
                );
            });

            addInvisibleAnchor(model, "MoonOrbitAnchor", [&](auto& renderUnit) {
                renderUnit.parent("EarthTiltAnchor");
                renderUnit.animation(
                    EllipticOrbiting(Animation::LOOP)
                        .semiMajorAxis(moonSemiMajor)
                        .semiMinorAxis(moonSemiMinor)
                        .duration(orbitDurationFromDays(moon))
                        .useFocusOrigin(true)
                );
            });

            addTiltAnchor(model, "MoonTiltAnchor", "MoonOrbitAnchor", moon.axialTiltDegrees);

            model.renderUnit("Moon", [&](auto& renderUnit) {
                renderUnit.parent("MoonTiltAnchor");
                renderUnit.mesh(Sphere(moonRadius));
                renderUnit.texture(moon.textureFile);
                renderUnit.animation(
                    Rotation(Animation::LOOP)
                        .duration(orbitDurationFromDays(moon))
                        .rotationAxis({0.0f, -1.0f, 0.0f})
                );
            });
        });
    }


    void addOrbitingBodySystem(Scene& scene, const CelestialBodyData& body) {
        if (body.name == "Saturn") return addSaturnSystem(scene);
        if (body.name == "Earth") return addEarthMoonSystem(scene);

        const float radius = displayedRadius(body);
        const float semiMajor = displayedOrbitSemiMajor(body);
        const float semiMinor = displayedOrbitSemiMinor(body);

        const std::string systemName = body.name + "System";
        const std::string orbitAnchor = body.name + "OrbitAnchor";
        const std::string tiltAnchor = body.name + "TiltAnchor";

        scene.model(systemName, [&](auto& model) {
            addInvisibleAnchor(model, orbitAnchor, [&](auto& renderUnit) {
                renderUnit.animation(
                    EllipticOrbiting(Animation::LOOP)
                        .semiMajorAxis(semiMajor)
                        .semiMinorAxis(semiMinor)
                        .duration(orbitDurationFromDays(body))
                        .useFocusOrigin(true)
                );
            });

            addTiltAnchor(model, tiltAnchor, orbitAnchor, body.axialTiltDegrees);

            model.renderUnit(body.name, [&](auto& renderUnit) {
                renderUnit.parent(tiltAnchor);
                renderUnit.mesh(Sphere(radius));
                renderUnit.texture(body.textureFile);
                renderUnit.animation(
                    Rotation(Animation::LOOP)
                        .duration(spinDurationFromDays(body))
                        .rotationAxis(spinAxisFor(body))
                );
            });
        });
    }

} // namespace


std::shared_ptr<pixelforge::scene::nodes::Scene> buildSolarSystemDemo() {
    const CelestialBodyData sun = celestialBodies.at("Sun");
    const float sunRadius = displayedRadius(sun);
    auto sunMaterial = std::make_shared<Material>(sun.textureFile);
    sunMaterial->setEmissiveMap(sunMaterial->getDiffuseMap());

    auto scene = Scene("Solar System")
        .skySphere("solar_system/stars_milky_way.jpg")
        .lightUnit("SunLight", [](auto& lightUnit) {
            lightUnit.light(
                PositionalLight()
                    .ambient({1.0f, 0.95f, 0.85f}, 0.2f)
                    .diffuse({1.0f, 0.95f, 0.85f}, 2.2f)
                    .specular({1.0f, 0.95f, 0.85f}, 1.4f)
                    .attenuation({1.0f, 0.0f, 0.0001f})
            );
            lightUnit.nodeScale(0.2f);
        })
        .model("SunSystem", [&](auto& model) {
            model.renderUnit("Sun", [&](auto& renderUnit) {
                renderUnit.mesh(Sphere(sunRadius));
                renderUnit.material(sunMaterial);
                renderUnit.animation(
                    Rotation(Animation::LOOP)
                        .duration(spinDurationFromDays(sun))
                );
            });
        });

    addOrbitingBodySystem(scene, celestialBodies.at("Mercury"));
    addOrbitingBodySystem(scene, celestialBodies.at("Venus"));
    addOrbitingBodySystem(scene, celestialBodies.at("Mars"));
    addOrbitingBodySystem(scene, celestialBodies.at("Earth"));
    addOrbitingBodySystem(scene, celestialBodies.at("Jupiter"));
    addOrbitingBodySystem(scene, celestialBodies.at("Saturn"));
    addOrbitingBodySystem(scene, celestialBodies.at("Uranus"));
    addOrbitingBodySystem(scene, celestialBodies.at("Neptune"));

    return scene;
}
