add_executable(pixelforge-demo main.cpp)
target_link_libraries(pixelforge-demo PRIVATE pixelforge::pixelforge)

pixelforge_enable_warnings(pixelforge-demo)