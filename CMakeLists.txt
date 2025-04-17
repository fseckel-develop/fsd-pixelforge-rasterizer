cmake_minimum_required(VERSION 3.29)
project(PixelForge)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Setting Header Files
set(HEADER_FILES
        Engine/Core.h
        Engine/Utilities.h
)


# Setting Source Files
set(SOURCE_FILES
        # Core
        Engine/Core/Application.cpp
        Engine/Core/Camera.cpp
        Engine/Core/Input.cpp
        Engine/Core/Window.cpp
)


# Adding main Executable
add_executable(PixelForge
        main.cpp
        ${HEADER_FILES}
        ${SOURCE_FILES}
)


# Including Directories for Dependencies
target_include_directories(PixelForge PRIVATE
        ${CMAKE_SOURCE_DIR}/dependencies/EIGEN/include
        ${CMAKE_SOURCE_DIR}/dependencies/GLEW/include
        ${CMAKE_SOURCE_DIR}/dependencies/GLFW/include
        ${CMAKE_SOURCE_DIR}/dependencies/GLM/include
        ${CMAKE_SOURCE_DIR}/dependencies/IMGUI/include
        ${CMAKE_SOURCE_DIR}/dependencies/SOIL2/include
)


# Defining Paths for Libraries
set(GLEW_LIB ${CMAKE_SOURCE_DIR}/dependencies/GLEW/lib/libGLEW.a)
set(GLFW_LIB ${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib/libglfw3.a)
set(SOIL2_LIB ${CMAKE_SOURCE_DIR}/dependencies/SOIL2/lib/libsoil2.a)


# Debugging Messages
message(STATUS "GLEW Library: ${GLEW_LIB}")
message(STATUS "GLFW Library: ${GLFW_LIB}")
message(STATUS "SOIL2 Library: ${SOIL2_LIB}")


# Linking Libraries
find_package(OpenGL REQUIRED)
target_link_libraries(PixelForge PRIVATE
        ${GLEW_LIB}
        ${GLFW_LIB}
        ${SOIL2_LIB}
        OpenGL::GL
)


# Platform-specific Setup
if (APPLE)
    target_compile_options(PixelForge PRIVATE -Wno-deprecated-declarations)
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
    find_library(IOKIT_FRAMEWORK IOKit REQUIRED)
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation REQUIRED)
    find_library(COREVIDEO_FRAMEWORK CoreVideo REQUIRED)
    target_link_libraries(PixelForge PRIVATE
            ${COCOA_FRAMEWORK}
            ${IOKIT_FRAMEWORK}
            ${COREFOUNDATION_FRAMEWORK}
            ${COREVIDEO_FRAMEWORK}
    )
endif ()
