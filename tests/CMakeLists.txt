cmake_minimum_required(VERSION 3.25)

set(PIXELFORGE_TEST_COMMON_SOURCES
        test_main.cpp

        builders/animation_builder_compiler_tests.cpp
        builders/light_builder_compiler_tests.cpp
        builders/light_unit_builder_compiler_tests.cpp
        builders/model_builder_compiler_tests.cpp
        builders/render_unit_builder_compiler_tests.cpp
        builders/scene_builder_compiler_tests.cpp

        geometry/curves/bezier_curve_tests.cpp
        geometry/curves/bspline_tests.cpp
        geometry/curves/hermite_spline_tests.cpp
        geometry/curves/lagrange_curve_tests.cpp
        geometry/curves/nurbs_tests.cpp
        geometry/meshes/cuboid_tests.cpp
        geometry/meshes/sphere_tests.cpp
        geometry/meshes/torus_tests.cpp
        geometry/utilities_tests.cpp

        graphics/pipeline/vertex_data_tests.cpp

        integration/animation_transform_pipeline_tests.cpp
        integration/builder_scene_pipeline_tests.cpp

        managers/animation_manager_tests.cpp
        managers/curve_manager_tests.cpp
        managers/light_manager_tests.cpp
        managers/material_manager_tests.cpp
        managers/mesh_manager_tests.cpp
        managers/scene_node_manager_tests.cpp

        scene/animation/animation_tests.cpp
        scene/animation/curve_animation_tests.cpp
        scene/animation/keyframe_animation_tests.cpp
        scene/animation/orbiting_tests.cpp
        scene/animation/rotation_tests.cpp
        scene/animation/scaling_tests.cpp
        scene/animation/translation_tests.cpp

        scene/lighting/ambient_light_tests.cpp
        scene/lighting/directional_light_tests.cpp
        scene/lighting/light_tests.cpp
        scene/lighting/positional_light_tests.cpp
        scene/lighting/spot_light_tests.cpp

        scene/nodes/animation_node_tests.cpp
        scene/nodes/light_unit_tests.cpp
        scene/nodes/model_tests.cpp
        scene/nodes/render_unit_tests.cpp
        scene/nodes/scene_node_tests.cpp
        scene/nodes/scene_tests.cpp
        scene/nodes/transform_node_tests.cpp

        scene/transform/rotate_tests.cpp
        scene/transform/scale_tests.cpp
        scene/transform/transform_tests.cpp
        scene/transform/translate_tests.cpp

        scene/utilities_tests.cpp
)

add_executable(pixelforge-tests
        ${PIXELFORGE_TEST_COMMON_SOURCES}
)

target_include_directories(pixelforge-tests PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(pixelforge-tests PRIVATE
        pixelforge::pixelforge
        doctest
)

add_test(NAME pixelforge-tests COMMAND pixelforge-tests)


if(PIXELFORGE_BUILD_GL_TESTS)
    set(PIXELFORGE_TEST_GL_SOURCES
            test_main_gl_context.cpp

            core/camera_tests.cpp
            core/window_tests.cpp

            graphics/pipeline/index_buffer_tests.cpp
            graphics/pipeline/shader_tests.cpp
            graphics/pipeline/vertex_array_tests.cpp
            graphics/pipeline/vertex_buffer_layout_tests.cpp
            graphics/pipeline/vertex_buffer_tests.cpp

            graphics/texturing/cube_map_tests.cpp
            graphics/texturing/material_tests.cpp
            graphics/texturing/texture_tests.cpp

            integration/material_texture_pipeline_tests.cpp
            integration/scene_cube_map_tests.cpp

            managers/texture_manager_tests.cpp
    )

    add_executable(pixelforge-tests-gl
            ${PIXELFORGE_TEST_GL_SOURCES}
    )

    target_include_directories(pixelforge-tests-gl PRIVATE
            ${CMAKE_SOURCE_DIR}/src
    )

    target_link_libraries(pixelforge-tests-gl PRIVATE
            pixelforge::pixelforge
            doctest
            glfw
    )

    add_test(NAME pixelforge-tests-gl COMMAND pixelforge-tests-gl)
endif()