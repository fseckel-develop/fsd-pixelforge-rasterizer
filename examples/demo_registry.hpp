#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace pixelforge::scene::nodes { class Scene; }

namespace pixelforge::examples {

    using SceneFactory = std::function<std::shared_ptr<scene::nodes::Scene>()>;

    struct DemoEntry {
        std::string key;
        std::string description;
        SceneFactory factory;
    };

    /// Returns all registered demo scenes.
    [[nodiscard]] const std::vector<DemoEntry>& demoRegistry();

    /// Looks up a demo scene factory by key.
    /// @param key Registry key such as "basic-scene" or "solar-system".
    /// @return Matching factory, or nullptr if no matching entry exists.
    [[nodiscard]] SceneFactory findDemoFactory(const std::string& key);

} // namespace pixelforge::examples
