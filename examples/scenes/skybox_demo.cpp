#include "skybox_demo.hpp"
#include <pixelforge/builders/scene.hpp>
#include <vector>
#include <random>


std::shared_ptr<pixelforge::scene::nodes::Scene> buildSkyBoxDemo() {
    static std::vector<std::string> skyBoxes = {
        "aurora1", "aurora2", "aurora3", "aurora4", "aurora5", "aurora7",
        "beksinskisky1", "beksinskisky2", "beksinskisky3", "bloodmoon",
        "bluesky1", "bluesky2", "bluesky3", "cloudysky1", "cloudysky2",
        "cloudysky4", "cloudysky5", "cloudysky6", "greensky1", "nightsky1",
        "nightsky2", "nightsky3", "purplesky1", "purplesky2", "purplesky3",
        "purplesky4", "purplesky5", "purplesky6", "purplesky7", "purplesky8",
        "sunset1", "sunset2", "sunset3", "tealsky"
    };
    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());
    std::uniform_int_distribution distribution(0, static_cast<int>(skyBoxes.size()) - 1);
    const std::string& chosen = skyBoxes[static_cast<size_t>(distribution(generator))];
    return pixelforge::builders::Scene("Skybox Demo").cubeMap(chosen);
}
