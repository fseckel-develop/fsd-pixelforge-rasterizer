#include "demo_registry.hpp"
#include "scenes/basic_demo.hpp"
#include "scenes/skybox_demo.hpp"
#include "scenes/solar_system_demo.hpp"


namespace pixelforge::examples {

    const std::vector<DemoEntry>& demoRegistry() {
        static const std::vector<DemoEntry> registry = {
            {
                "basic-scene",
                "Basic scene graph and animation showcase",
                buildBasicSceneDemo
            },
            {
                "skybox-demo",
                "Skybox and cube map showcase",
                buildSkyBoxDemo
            }
        };
        return registry;
    }

    SceneFactory findDemoFactory(const std::string& key) {
        for (const auto& entry : demoRegistry()) {
            if (entry.key == key) {
                return entry.factory;
            }
        }
        return nullptr;
    }

} // namespace pixelforge::examples
