#include <pixelforge/core/application.hpp>
#include <pixelforge/builders.hpp>
#include <pixelforge/geometry.hpp>
using namespace pixelforge::builders;


std::shared_ptr<pixelforge::scene::nodes::Scene> buildDemoScene() {
    const auto scene = SceneBuilder("Scene")
        .with(ModelBuilder("Model")
            .with(LightUnitBuilder("Ambient")
                .withLight(AmbientLightBuilder().withIntensity(0.5f)))
            .with(LightUnitBuilder("Light")
                .withLight(PositionalLightBuilder())
                .withAnimation(OrbitingBuilder(pixelforge::scene::animation::Animation::LOOP)
                    .withDuration(3.0f).withRadius(4.0f).withRotationAxis({0.0f, -1.0f, 0.0f}))
                .withNodeScale(0.15f))
            .with(RenderUnitBuilder("Sphere")
                .withMesh(pixelforge::geometry::Sphere())
                .withMaterial(pixelforge::graphics::Gold())
                .withAnimation(RotationBuilder(pixelforge::scene::animation::Animation::LOOP)
                    .withDuration(5.0f).withRotationAxis({1.0f, 0.0f, 1.0f}))
                .withAnimation(OrbitingBuilder(pixelforge::scene::animation::Animation::LOOP)
                    .withDuration(8.0f).withRadius(2.0f).withRotationAxis({0.0f, 1.0f, 0.0f})))
            .with(RenderUnitBuilder("Cube", "Sphere")
                .withMesh("Cube.obj")
                .withMaterial(pixelforge::graphics::Silver())
                .withTransform(pixelforge::scene::transform::Scale(0.5f))
                .withAnimation(OrbitingBuilder(pixelforge::scene::animation::Animation::LOOP)
                    .withDuration(4.0f).withRadius(2.0f).withRotationAxis({0.5, 1.5f, -2.0})))
            .with(RenderUnitBuilder("Cylinder", "Cube")
                .withMesh("Cylinder.obj")
                .withMaterial(pixelforge::graphics::Bronze())
                .withNodeScale(0.5f)
                .withAnimation(OrbitingBuilder(pixelforge::scene::animation::Animation::LOOP)
                    .withDuration(6.0f).withRadius(1.5f).withRotationAxis({0.5, -1.5f, 2.0}))))
        .build();
    return scene;
}


int main() {
    pixelforge::core::Application::run([] {
        return buildDemoScene();
    });
}
