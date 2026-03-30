# Header-only: GLM
add_library(glm INTERFACE)
add_library(glm::glm ALIAS glm)
target_include_directories(glm INTERFACE
        ${PROJECT_SOURCE_DIR}/external/glm/include
)

# Header-only: Eigen
add_library(eigen INTERFACE)
add_library(eigen::eigen ALIAS eigen)
target_include_directories(eigen INTERFACE
        ${PROJECT_SOURCE_DIR}/external/eigen/include
)

# Header-only: doctest
add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE
        ${PROJECT_SOURCE_DIR}/external/doctest
)

# Header-only: stb_image
add_library(stb_image INTERFACE)
add_library(stb::stb ALIAS stb_image)
target_include_directories(stb_image INTERFACE
        ${PROJECT_SOURCE_DIR}/external/stb_image
)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

if(APPLE)
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
    find_library(IOKIT_FRAMEWORK IOKit REQUIRED)
    find_library(COREVIDEO_FRAMEWORK CoreVideo REQUIRED)
endif()