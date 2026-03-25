set(PIXELFORGE_SOURCES
        core/application.cpp
        core/camera.cpp
        core/input.cpp
        core/renderer.cpp
        core/window.cpp

        geometry/curves/bezier_curve.cpp
        geometry/curves/bspline.cpp
        geometry/curves/curve.cpp
        geometry/curves/function_curve.cpp
        geometry/curves/hermite_spline.cpp
        geometry/curves/lagrange_curve.cpp
        geometry/curves/nurbs.cpp

        geometry/meshes/arrow.cpp
        geometry/meshes/cone.cpp
        geometry/meshes/cube.cpp
        geometry/meshes/cuboid.cpp
        geometry/meshes/cylinder.cpp
        geometry/meshes/frustum.cpp
        geometry/meshes/mesh.cpp
        geometry/meshes/points.cpp
        geometry/meshes/polygon.cpp
        geometry/meshes/rounded_cube.cpp
        geometry/meshes/shell.cpp
        geometry/meshes/sphere.cpp
        geometry/meshes/sphere_sector.cpp
        geometry/meshes/sphere_segment.cpp
        geometry/meshes/sphere_wedge.cpp
        geometry/meshes/torus.cpp
        geometry/meshes/tube.cpp

        graphics/pipeline/index_buffer.cpp
        graphics/pipeline/shader.cpp
        graphics/pipeline/vertex_array.cpp
        graphics/pipeline/vertex_buffer.cpp
        graphics/pipeline/vertex_buffer_layout.cpp
        graphics/pipeline/vertex_data.cpp

        graphics/texturing/cube_map.cpp
        graphics/texturing/material.cpp
        graphics/texturing/texture.cpp

        scene/animation/animation.cpp
        scene/animation/curve_animation.cpp
        scene/animation/keyframe_animation.cpp
        scene/animation/orbiting.cpp
        scene/animation/rotation.cpp
        scene/animation/scaling.cpp
        scene/animation/translation.cpp

        scene/builders/detail/scene_node_registry.cpp

        scene/lighting/ambient_light.cpp
        scene/lighting/directional_light.cpp
        scene/lighting/light.cpp
        scene/lighting/positional_light.cpp
        scene/lighting/spot_light.cpp

        scene/nodes/animation_node.cpp
        scene/nodes/group.cpp
        scene/nodes/light_unit.cpp
        scene/nodes/model.cpp
        scene/nodes/render_unit.cpp
        scene/nodes/scene.cpp
        scene/nodes/scene_node.cpp
        scene/nodes/transform_node.cpp

        scene/transform/rotate.cpp
        scene/transform/scale.cpp
        scene/transform/transform.cpp
        scene/transform/translate.cpp
)

add_library(pixelforge STATIC)
add_library(pixelforge::pixelforge ALIAS pixelforge)

target_sources(pixelforge PRIVATE ${PIXELFORGE_SOURCES})

target_compile_features(pixelforge PUBLIC cxx_std_20)

target_include_directories(pixelforge
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(pixelforge
        PUBLIC
        OpenGL::GL
        glm::glm
        glew::glew
        PRIVATE
        eigen::eigen
        glfw::glfw
        soil2::soil2
)

if(APPLE)
    target_link_libraries(pixelforge PRIVATE
            ${COCOA_FRAMEWORK}
            ${IOKIT_FRAMEWORK}
            ${COREVIDEO_FRAMEWORK}
    )
endif()

target_compile_definitions(pixelforge PUBLIC
        $<BUILD_INTERFACE:PIXELFORGE_ASSET_DIR="${PROJECT_SOURCE_DIR}/assets">
        $<INSTALL_INTERFACE:PIXELFORGE_ASSET_DIR="${CMAKE_INSTALL_FULL_DATADIR}/pixelforge/assets">
)

pixelforge_enable_warnings(pixelforge)