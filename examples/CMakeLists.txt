add_executable(pixelforge-demo
        main.cpp
        demo_registry.cpp
        scenes/basic_demo.cpp
        scenes/skybox_demo.cpp
)

target_include_directories(pixelforge-demo PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(pixelforge-demo PRIVATE
        pixelforge::pixelforge
)

pixelforge_enable_warnings(pixelforge-demo)