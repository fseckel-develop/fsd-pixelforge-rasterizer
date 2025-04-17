cmake_minimum_required(VERSION 3.29)
project(PixelForge)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setting Header Files
set(HEADER_FILES
        Engine/Core.h
        Engine/Geometry.h
        Engine/Graphics.h
        Engine/Scene.h
        Engine/Utilities.h
)

# Setting Source Files
set(SOURCE_FILES
        # Core
        Engine/Core/Application.cpp
        Engine/Core/Camera.cpp
        Engine/Core/Input.cpp
        Engine/Core/Menu.cpp
        Engine/Core/Renderer.cpp
        Engine/Core/Window.cpp

        # Geometry/Curves
        Engine/Geometry/Curves/BezierCurve.cpp
        Engine/Geometry/Curves/BSpline.cpp
        Engine/Geometry/Curves/Curve.cpp
        Engine/Geometry/Curves/FunctionCurve.cpp
        Engine/Geometry/Curves/HermiteSpline.cpp
        Engine/Geometry/Curves/LagrangeCurve.cpp
        Engine/Geometry/Curves/NURBS.cpp
        # Geometry/Meshes
        Engine/Geometry/Meshes/Arrow.cpp
        Engine/Geometry/Meshes/Cone.cpp
        Engine/Geometry/Meshes/Cube.cpp
        Engine/Geometry/Meshes/Cuboid.cpp
        Engine/Geometry/Meshes/Cylinder.cpp
        Engine/Geometry/Meshes/Frustum.cpp
        Engine/Geometry/Meshes/Mesh.cpp
        Engine/Geometry/Meshes/Points.cpp
        Engine/Geometry/Meshes/Polygon.cpp
        Engine/Geometry/Meshes/RoundedCube.cpp
        Engine/Geometry/Meshes/Shell.cpp
        Engine/Geometry/Meshes/Sphere.cpp
        Engine/Geometry/Meshes/SphereSector.cpp
        Engine/Geometry/Meshes/SphereSegment.cpp
        Engine/Geometry/Meshes/SphereWedge.cpp
        Engine/Geometry/Meshes/Torus.cpp
        Engine/Geometry/Meshes/Tube.cpp

        # Graphics/Pipeline
        Engine/Graphics/Pipeline/IndexBuffer.cpp
        Engine/Graphics/Pipeline/ShaderProgram.cpp
        Engine/Graphics/Pipeline/ShaderStorageBuffer.cpp
        Engine/Graphics/Pipeline/UniformBuffer.cpp
        Engine/Graphics/Pipeline/VertexArray.cpp
        Engine/Graphics/Pipeline/VertexBuffer.cpp
        Engine/Graphics/Pipeline/VertexBufferLayout.cpp
        Engine/Graphics/Pipeline/VertexData.cpp
        # Graphics/Texturing
        Engine/Graphics/Texturing/CubeMap.cpp
        Engine/Graphics/Texturing/Material.cpp
        Engine/Graphics/Texturing/Texture.cpp
        Engine/Graphics/Texturing/TextureUnitManager.cpp

        # Scene
        Engine/Scene/Scene.cpp
        # Scene/Animations
        Engine/Scene/Animations/Animation.cpp
        Engine/Scene/Animations/CurveAnimation.cpp
        Engine/Scene/Animations/Easing.cpp
        Engine/Scene/Animations/KeyframeAnimation.cpp
        Engine/Scene/Animations/Orbiting.cpp
        Engine/Scene/Animations/Rotation.cpp
        Engine/Scene/Animations/Scaling.cpp
        Engine/Scene/Animations/Translation.cpp
        # Scene/Lights
        Engine/Scene/Lights/AmbientLight.cpp
        Engine/Scene/Lights/DirectionalLight.cpp
        Engine/Scene/Lights/PositionalLight.cpp
        Engine/Scene/Lights/SpotLight.cpp
        # Scene/SceneNodes
        Engine/Scene/SceneNodes/Light.cpp
        Engine/Scene/SceneNodes/Model.cpp
        Engine/Scene/SceneNodes/Renderable.cpp
        Engine/Scene/SceneNodes/SceneNode.cpp
        # Scene/Transforms
        Engine/Scene/Transforms/Rotate.cpp
        Engine/Scene/Transforms/Scale.cpp
        Engine/Scene/Transforms/Transform.cpp
        Engine/Scene/Transforms/Translate.cpp
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

# Pedantic compilation
target_compile_options(PixelForge PRIVATE -Wall -Wextra -pedantic)

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
