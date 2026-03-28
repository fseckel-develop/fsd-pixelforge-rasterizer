#include "basic_demo.hpp"
#include <pixelforge/builders/scene.hpp>
#include <pixelforge/graphics.hpp>
#include <pixelforge/scene/animation/animation.hpp>
#include <pixelforge/scene/transform/scale.hpp>
#include <pixelforge/scene/transform/translate.hpp>
#include <pixelforge/scene/utilities.hpp>

using pixelforge::builders::AmbientLight;
using pixelforge::builders::EllipticOrbiting;
using pixelforge::builders::Orbiting;
using pixelforge::builders::PositionalLight;
using pixelforge::builders::Rotation;
using pixelforge::builders::Scaling;
using pixelforge::builders::Scene;
using pixelforge::builders::Translation;


std::shared_ptr<pixelforge::scene::nodes::Scene> buildBasicSceneDemo() {
    return Scene("Basic Scene")
        .lightUnit("Ambient", [](auto& lightUnit) {
            lightUnit.light(
                AmbientLight().intensity(0.20f)
            );
        })

        .model("Model", [](auto& model) {
            model.lightUnit("LightUpper", [](auto& lightUnit) {
                lightUnit.light(PositionalLight());
                lightUnit.mesh("Sphere.obj");
                lightUnit.nodeScale(0.10f);
                lightUnit.transform(
                    pixelforge::scene::transform::Translate{0.0f, 1.8f, 0.0f}
                );
                lightUnit.animation(
                    Orbiting(pixelforge::scene::animation::Animation::LOOP)
                        .duration(7.0f)
                        .radius(6.5f)
                        .rotationAxis({0.0f, 1.0f, 0.0f})
                );
                lightUnit.animation(
                    Translation(pixelforge::scene::animation::Animation::BOUNCE)
                        .duration(2.8f)
                        .direction({0.0f, 1.0f, 0.0f})
                        .totalDistance(1.0f)
                );
            });

            model.lightUnit("LightLower", [](auto& lightUnit) {
                lightUnit.light(PositionalLight());
                lightUnit.mesh("Sphere.obj");
                lightUnit.nodeScale(0.10f);
                lightUnit.transform(
                    pixelforge::scene::transform::Translate{0.0f, -1.8f, 0.0f}
                );
                lightUnit.animation(
                    Orbiting(pixelforge::scene::animation::Animation::LOOP)
                        .duration(7.0f)
                        .radius(6.5f)
                        .rotationAxis({0.0f, -1.0f, 0.0f})
                );
                lightUnit.animation(
                    Translation(pixelforge::scene::animation::Animation::BOUNCE)
                        .duration(2.8f)
                        .direction({0.0f, -1.0f, 0.0f})
                        .totalDistance(1.0f)
                );
            });

            model.renderUnit("Core", [](auto& renderUnit) {
                renderUnit.mesh("Sphere.obj");
                renderUnit.material(pixelforge::graphics::Gold());
                renderUnit.nodeScale(0.85f);

                renderUnit.animation(
                    Rotation(pixelforge::scene::animation::Animation::LOOP)
                        .duration(10.0f)
                        .rotationAxis({0.0f, 1.0f, 0.0f})
                );

                renderUnit.animation(
                    Scaling(pixelforge::scene::animation::Animation::BOUNCE)
                        .duration(4.0f)
                        .targetScale(1.08f)
                );
            });

            model.renderUnit("InnerDiskLeft", [](auto& renderUnit) {
                renderUnit.parent("Core");
                renderUnit.mesh("Sphere.obj");
                renderUnit.material(pixelforge::graphics::Ruby());
                renderUnit.transform(
                    pixelforge::scene::transform::Translate{-2.8f, 0.0f, 0.0f}
                );
                renderUnit.nodeScale({0.95f, 0.22f, 0.95f});

                renderUnit.animation(
                    Orbiting(pixelforge::scene::animation::Animation::LOOP)
                        .duration(6.0f)
                        .radius(2.8f)
                        .rotationAxis({0.0f, 1.0f, 0.0f})
                );

                renderUnit.animation(
                    Rotation(pixelforge::scene::animation::Animation::LOOP)
                        .duration(3.0f)
                        .rotationAxis({1.0f, 0.0f, 0.0f})
                );

                renderUnit.animation(
                    Translation(pixelforge::scene::animation::Animation::BOUNCE)
                        .duration(2.5f)
                        .direction({0.0f, 0.6f, 0.0f})
                        .totalDistance(0.6f)
                );
            });

            model.renderUnit("InnerDiskRight", [](auto& renderUnit) {
                renderUnit.parent("Core");
                renderUnit.mesh("Sphere.obj");
                renderUnit.material(pixelforge::graphics::Emerald());
                renderUnit.transform(
                    pixelforge::scene::transform::Translate{2.8f, 0.0f, 0.0f}
                );
                renderUnit.nodeScale({0.95f, 0.22f, 0.95f});

                renderUnit.animation(
                    Orbiting(pixelforge::scene::animation::Animation::LOOP)
                        .duration(6.0f)
                        .radius(2.8f)
                        .rotationAxis({0.0f, -1.0f, 0.0f})
                );

                renderUnit.animation(
                    Rotation(pixelforge::scene::animation::Animation::LOOP)
                        .duration(3.0f)
                        .rotationAxis({1.0f, 0.0f, 0.0f})
                );

                renderUnit.animation(
                    Translation(pixelforge::scene::animation::Animation::BOUNCE)
                        .duration(2.5f)
                        .direction({0.0f, -0.6f, 0.0f})
                        .totalDistance(0.6f)
                );
            });

            model.renderUnit("FrontOval", [](auto& renderUnit) {
                renderUnit.parent("Core");
                renderUnit.mesh("Sphere.obj");
                renderUnit.material(pixelforge::graphics::Silver());
                renderUnit.transform(
                    pixelforge::scene::transform::Translate{0.0f, 0.0f, 4.3f}
                );
                renderUnit.nodeScale({0.55f, 1.10f, 0.55f});

                renderUnit.animation(
                    EllipticOrbiting(pixelforge::scene::animation::Animation::LOOP)
                        .duration(8.0f)
                        .axes(4.3f, 3.2f)
                        .rotationAxis({0.0f, 1.0f, 0.0f})
                );

                renderUnit.animation(
                    Rotation(pixelforge::scene::animation::Animation::LOOP)
                        .duration(4.0f)
                        .rotationAxis({0.0f, 0.0f, 1.0f})
                );
            });

            model.renderUnit("BackOval", [](auto& renderUnit) {
                renderUnit.parent("Core");
                renderUnit.mesh("Sphere.obj");
                renderUnit.material(pixelforge::graphics::Bronze());
                renderUnit.transform(
                    pixelforge::scene::transform::Translate{0.0f, 0.0f, -4.3f}
                );
                renderUnit.nodeScale({0.55f, 1.10f, 0.55f});

                renderUnit.animation(
                    EllipticOrbiting(pixelforge::scene::animation::Animation::LOOP)
                        .duration(8.0f)
                        .axes(4.3f, 3.2f)
                        .rotationAxis({0.0f, -1.0f, 0.0f})
                );

                renderUnit.animation(
                    Rotation(pixelforge::scene::animation::Animation::LOOP)
                        .duration(4.0f)
                        .rotationAxis({0.0f, 0.0f, -1.0f})
                );
            });

            model.renderUnit("SatelliteA", [](auto& renderUnit) {
                renderUnit.parent("Core");
                renderUnit.mesh("Sphere.obj");
                renderUnit.material(pixelforge::graphics::Pearl());
                renderUnit.transform(
                    pixelforge::scene::transform::Translate{1.9f, 1.0f, 0.0f}
                );
                renderUnit.nodeScale(0.20f);

                renderUnit.animation(
                    Orbiting(pixelforge::scene::animation::Animation::LOOP)
                        .duration(3.2f)
                        .radius(2.2f)
                        .rotationAxis({1.0f, 0.0f, 0.0f})
                );
            });

            model.renderUnit("SatelliteB", [](auto& renderUnit) {
                renderUnit.parent("Core");
                renderUnit.mesh("Sphere.obj");
                renderUnit.material(pixelforge::graphics::Turquoise());
                renderUnit.transform(
                    pixelforge::scene::transform::Translate{-1.9f, 1.0f, 0.0f}
                );
                renderUnit.nodeScale(0.20f);

                renderUnit.animation(
                    Orbiting(pixelforge::scene::animation::Animation::LOOP)
                        .duration(3.2f)
                        .radius(2.2f)
                        .rotationAxis({-1.0f, 0.0f, 0.0f})
                );
            });

            model.renderUnit("SatelliteC", [](auto& renderUnit) {
                renderUnit.parent("Core");
                renderUnit.mesh("Sphere.obj");
                renderUnit.material(pixelforge::graphics::Chrome());
                renderUnit.transform(
                    pixelforge::scene::transform::Translate{0.0f, 1.0f, 1.9f}
                );
                renderUnit.nodeScale(0.20f);

                renderUnit.animation(
                    Orbiting(pixelforge::scene::animation::Animation::LOOP)
                        .duration(3.2f)
                        .radius(2.2f)
                        .rotationAxis({0.0f, 0.0f, 1.0f})
                );
            });

            model.renderUnit("SatelliteD", [](auto& renderUnit) {
                renderUnit.parent("Core");
                renderUnit.mesh("Sphere.obj");
                renderUnit.material(pixelforge::graphics::Copper());
                renderUnit.transform(
                    pixelforge::scene::transform::Translate{0.0f, -1.0f, -1.9f}
                );
                renderUnit.nodeScale(0.20f);

                renderUnit.animation(
                    Orbiting(pixelforge::scene::animation::Animation::LOOP)
                        .duration(3.2f)
                        .radius(2.2f)
                        .rotationAxis({0.0f, 0.0f, -1.0f})
                );
            });

            model.renderUnit("HaloLensLeft", [](auto& renderUnit) {
                renderUnit.parent("Core");
                renderUnit.mesh("Sphere.obj");
                renderUnit.material(pixelforge::graphics::Obsidian());
                renderUnit.transform(
                    pixelforge::scene::transform::Translate{-6.2f, 0.0f, 0.0f}
                );
                renderUnit.nodeScale({0.75f, 0.16f, 0.75f});

                renderUnit.animation(
                    Orbiting(pixelforge::scene::animation::Animation::LOOP)
                        .duration(14.0f)
                        .radius(6.2f)
                        .rotationAxis({0.0f, 1.0f, 0.0f})
                );
            });

            model.renderUnit("HaloLensRight", [](auto& renderUnit) {
                renderUnit.parent("Core");
                renderUnit.mesh("Sphere.obj");
                renderUnit.material(pixelforge::graphics::Jade());
                renderUnit.transform(
                    pixelforge::scene::transform::Translate{6.2f, 0.0f, 0.0f}
                );
                renderUnit.nodeScale({0.75f, 0.16f, 0.75f});

                renderUnit.animation(
                    Orbiting(pixelforge::scene::animation::Animation::LOOP)
                        .duration(14.0f)
                        .radius(6.2f)
                        .rotationAxis({0.0f, -1.0f, 0.0f})
                );
            });
        });
}
