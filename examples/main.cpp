#include <pixelforge/core/application.hpp>
#include <pixelforge/scene/utilities.hpp>
#include <demo_registry.hpp>
#include <iostream>
#include <string>

using pixelforge::core::Application;


namespace {

    void printAvailableDemos() {
        std::cout << "Available demos:\n";
        for (const auto& entry : pixelforge::examples::demoRegistry()) {
            std::cout << "  " << entry.key << " - " << entry.description << "\n";
        }
    }

    std::string promptUserForDemo() {
        const auto& registry = pixelforge::examples::demoRegistry();
        std::cout << "\nSelect Demo:\n";
        for (size_t i = 0; i < registry.size(); ++i) {
            std::cout << "  [" << i << "] "
                      << registry[i].key << " - "
                      << registry[i].description << std::endl;
        }
        std::cout << "\nEnter number, name, or 'q' to quit: ";
        std::string input;
        std::getline(std::cin, input);
        if (input == "q" || input == "quit" || input == "exit") {
            return "";
        }
        try {
            if (const size_t index = std::stoul(input); index < registry.size()) {
                return registry[index].key;
            }
        } catch (...) { /* not a number → ignore */ }
        return input;
    }

} // namespace


int main(const int argc, char** argv) {
    std::string selectedDemo;
    if (argc >= 2) {
        const std::string argument = argv[1];
        if (argument == "--list" || argument == "-l") {
            printAvailableDemos();
            return 0;
        }
        selectedDemo = argument;
        // If invalid → fall back to prompt
        if (!pixelforge::examples::findDemoFactory(selectedDemo)) {
            std::cerr << "Unknown demo: " << selectedDemo << "\n";
            selectedDemo = promptUserForDemo();
        }
    } else {
        // No argument → interactive selection
        selectedDemo = promptUserForDemo();
    }
    if (selectedDemo.empty()) {
        std::cout << "Aborted.\n";
        return 0;
    }
    const auto factory = pixelforge::examples::findDemoFactory(selectedDemo);
    if (!factory) {
        std::cerr << "Invalid demo: " << selectedDemo << "\n";
        return 1;
    }
    Application::run([&factory] {
        const auto scene = factory();
        pixelforge::scene::utilities::printSceneSummary(scene);
        pixelforge::scene::utilities::printSceneNodeTree(scene);
        return scene;
    });
    return 0;
}
