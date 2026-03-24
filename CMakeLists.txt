cmake_minimum_required(VERSION 3.25)

project(fsd-pixelforge-rasterizer
        VERSION 0.1.0
        DESCRIPTION "Modern C++ rasterizer and graphics playground"
        LANGUAGES CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(PIXELFORGE_BUILD_EXAMPLES "Build PixelForge examples" ON)
option(PIXELFORGE_BUILD_TESTS "Build PixelForge tests" ON)
option(PIXELFORGE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(PIXELFORGE_INSTALL "Enable install and package export" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(Dependencies)
include(CompilerWarnings)

add_subdirectory(src)

if(PIXELFORGE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(PIXELFORGE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(PIXELFORGE_INSTALL)
    install(TARGETS pixelforge
            EXPORT pixelforgeTargets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(DIRECTORY include/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(DIRECTORY assets/
            DESTINATION ${CMAKE_INSTALL_DATADIR}/pixelforge
    )

    install(EXPORT pixelforgeTargets
            FILE pixelforgeTargets.cmake
            NAMESPACE pixelforge::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PixelForge
    )

    configure_package_config_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/cmake/PixelForgeConfig.cmake.in
            ${CMAKE_CURRENT_BINARY_DIR}/PixelForgeConfig.cmake
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PixelForge
    )

    write_basic_package_version_file(
            ${CMAKE_CURRENT_BINARY_DIR}/PixelForgeConfigVersion.cmake
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY SameMajorVersion
    )

    install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/PixelForgeConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/PixelForgeConfigVersion.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PixelForge
    )
endif()