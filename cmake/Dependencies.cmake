# Header-only: GLM
add_library(glm INTERFACE)
add_library(glm::glm ALIAS glm)
target_include_directories(glm INTERFACE
        ${PROJECT_SOURCE_DIR}/external/GLM/include
)

# Header-only: Eigen
add_library(eigen INTERFACE)
add_library(eigen::eigen ALIAS eigen)
target_include_directories(eigen INTERFACE
        ${PROJECT_SOURCE_DIR}/external/EIGEN/include
)

# Prebuilt: GLEW
add_library(glew STATIC IMPORTED)
add_library(glew::glew ALIAS glew)
set_target_properties(glew PROPERTIES
        IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/external/GLEW/lib/libGLEW.a"
        INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/external/GLEW/include"
)

# Prebuilt: GLFW
add_library(glfw STATIC IMPORTED)
add_library(glfw::glfw ALIAS glfw)
set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/external/GLFW/lib/libglfw3.a"
        INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/external/GLFW/include"
)

# Prebuilt: SOIL2
add_library(soil2 STATIC IMPORTED)
add_library(soil2::soil2 ALIAS soil2)
set_target_properties(soil2 PROPERTIES
        IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/external/SOIL2/lib/libsoil2.a"
        INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/external/SOIL2/include"
)

# Header-only: doctest
add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE
        ${PROJECT_SOURCE_DIR}/external/doctest
)

find_package(OpenGL REQUIRED)

if(APPLE)
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
    find_library(IOKIT_FRAMEWORK IOKit REQUIRED)
    find_library(COREVIDEO_FRAMEWORK CoreVideo REQUIRED)
endif()