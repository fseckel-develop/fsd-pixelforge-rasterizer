#include <pixelforge/builders.hpp>
#include <pixelforge/core/application.hpp>
#include <pixelforge/geometry.hpp>
#include <pixelforge/graphics.hpp>
#include <pixelforge/scene/animation/animation.hpp>
#include <pixelforge/scene/transform/scale.hpp>
#include <pixelforge/scene/utilities.hpp>

using pixelforge::builders::AmbientLight;
using pixelforge::builders::LightUnit;
using pixelforge::builders::Model;
using pixelforge::builders::Orbiting;
using pixelforge::builders::PositionalLight;
using pixelforge::builders::RenderUnit;
using pixelforge::builders::Rotation;
using pixelforge::builders::Scene;
using pixelforge::core::Application;


std::shared_ptr<pixelforge::scene::nodes::Scene> buildDemoScene() {
    return Scene("Scene")
        .lightUnit("Ambient", [](auto& lightUnit) {
            lightUnit.light(
                AmbientLight()
                    .intensity(0.5f)
            );
        })
        .model("Model", [](auto& model) {
            model.lightUnit("Light", [](auto& lightUnit) {
                lightUnit.light(
                    PositionalLight()
                );
                lightUnit.animation(
                    Orbiting(pixelforge::scene::animation::Animation::LOOP)
                        .duration(3.0f)
                        .radius(4.0f)
                        .rotationAxis({0.0f, -1.0f, 0.0f})
                );
                lightUnit.nodeScale(0.15f);
            });
            model.renderUnit("Sphere", [](auto& renderUnit) {
                renderUnit.mesh(pixelforge::geometry::Sphere());
                renderUnit.material(pixelforge::graphics::Gold());
                renderUnit.animation(
                    Rotation(pixelforge::scene::animation::Animation::LOOP)
                        .duration(5.0f)
                        .rotationAxis({1.0f, 0.0f, 1.0f})
                );
                renderUnit.animation(
                    Orbiting(pixelforge::scene::animation::Animation::LOOP)
                        .duration(8.0f)
                        .radius(2.0f)
                        .rotationAxis({0.0f, 1.0f, 0.0f})
                );
            });
            model.renderUnit("Cube", [](auto& renderUnit) {
                renderUnit.parent("Sphere");
                renderUnit.mesh("Cube.obj");
                renderUnit.material(pixelforge::graphics::Silver());
                renderUnit.transform(pixelforge::scene::transform::Scale(0.5f));
                renderUnit.animation(
                    Orbiting(pixelforge::scene::animation::Animation::LOOP)
                        .duration(4.0f)
                        .radius(2.0f)
                        .rotationAxis({0.5f, 1.5f, -2.0f})
                );
            });
            model.renderUnit("Cylinder", [](auto& renderUnit) {
                renderUnit.parent("Cube");
                renderUnit.mesh("Cylinder.obj");
                renderUnit.material(pixelforge::graphics::Bronze());
                renderUnit.nodeScale(0.5f);
                renderUnit.animation(
                    Orbiting(pixelforge::scene::animation::Animation::LOOP)
                        .duration(6.0f)
                        .radius(1.5f)
                        .rotationAxis({0.5f, -1.5f, 2.0f})
                );
            });
        })
        .cubeMap("test")
        .build();
}


int main() {
    Application::run([] {
        const auto scene = buildDemoScene();
        pixelforge::scene::utilities::printSceneSummary(scene);
        pixelforge::scene::utilities::printSceneNodeTree(scene);
        return scene;
    });
}
