cmake_minimum_required(VERSION 3.28)
project(glib)

set(CMAKE_CXX_STANDARD 20)

set(LOAD_DEPS True   CACHE STRING "Load dependencies")
set(LINKING   STATIC CACHE STRING "Linking Type")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

set(Apple   ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
set(Linux   ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
set(Windows ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

set(GraphicsAPI_DIR   "${CMAKE_SOURCE_DIR}/src/Graphics/GraphicsAPI")
set(RendererDIR       "${CMAKE_SOURCE_DIR}/src/Graphics/RendererCore")
set(FontGeneratorDIR  "${CMAKE_SOURCE_DIR}/src/FontGenerator")
set(LoggerDir         "${CMAKE_SOURCE_DIR}/src/Logger")
set(DrawDir           "${CMAKE_SOURCE_DIR}/src/Draw")

set(GLEW_LIB           "${CMAKE_SOURCE_DIR}/extdeps/glew" CACHE STRING "GLEW path")
set(GLFW_LIB           "${CMAKE_SOURCE_DIR}/extdeps/glfw")
set(GLM_LIB            "${CMAKE_SOURCE_DIR}/extdeps/glm")
set(STB_LIB            "${CMAKE_SOURCE_DIR}/extdeps/stb")
set(MSDFGEN_ATLAS_LIB  "${CMAKE_SOURCE_DIR}/extdeps/msdf-atlas-gen")

set(MSDF_ATLAS_BUILD_STANDALONE OFF)
set(MSDF_ATLAS_USE_VCPKG OFF)
set(MSDF_ATLAS_USE_SKIA OFF)

set(FREETYPE_LIBRARY "${CMAKE_SOURCE_DIR}/extdeps/freetype/lib/libfreetype.a")
set(FREETYPE_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/extdeps/freetype/include/freetype2")

add_executable(glib src/main.cpp
        src/FontGenerator/language_tile_set.cpp
        src/FontGenerator/font_tile.cpp
        src/FontGenerator/stb_tt.cpp
        src/FontGenerator/font.cpp
        src/FontGenerator/stb_iw.cpp
        src/Utils/camera.cpp
        src/Utils/atlas.cpp
        src/Utils/fullscreen_fbo.cpp
        src/Geometry/mesh.cpp
        src/Geometry/text.cpp
)


message(STATUS ${CMAKE_BUILD_TYPE})
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(glib PRIVATE
            $<$<CXX_COMPILER_ID:GNU,Clang>:-w>
            $<$<CXX_COMPILER_ID:MSVC>:/W0>
    )

    message(STATUS "Warnings turned off")
    add_compile_definitions("__GLIB_RELEASE__")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions("__GLIB_DEBUG__")
endif()

if (${Apple})
    message(STATUS "OS: MacOS")
    message(STATUS "ARCH: ${CMAKE_SYSTEM_PROCESSOR}")
    add_compile_definitions("MACOS")

    find_package(OpenGL REQUIRED)

    include_directories(glib
            "src/"
            "extdeps/"
            "${GLM_LIB}"
            "${STB_LIB}"
            "${GLEW_LIB}/include"
            "${GLFW_LIB}/include"
            "${MSDFGEN_ATLAS_LIB}"
            "${MSDFGEN_ATLAS_LIB}/msdfgen"
    )

    add_subdirectory(${GraphicsAPI_DIR})
    add_subdirectory(${MSDFGEN_ATLAS_LIB})
    add_subdirectory(${FontGeneratorDIR})
    add_subdirectory(${RendererDIR})
    add_subdirectory(${LoggerDir})
    add_subdirectory(${DrawDir})

    add_dependencies(glib msdf-atlas-gen)
    add_dependencies(glib GraphicsAPI)
    add_dependencies(glib FontGenerator)
    add_dependencies(glib RendererCore)
    add_dependencies(glib Logger)
    add_dependencies(glib Draw)

    set(GLFW_LIB_DIR "")
    if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/extdeps/glfw/lib-x86_64/libglfw3.a")
    elseif (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
        set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/extdeps/glfw/lib-arm64/libglfw3.a")
    endif ()

    target_link_libraries(glib
        "-framework Cocoa"
        "-framework IOKit"
        "${GLFW_LIB_DIR}"
        OpenGL::GL

        msdf-atlas-gen
        GraphicsAPI
        FontGenerator
        RendererCore
        Logger
        Draw
    )
elseif(${Linux})
    message("OS: Linux")
    message(STATUS "ARCH: ${CMAKE_SYSTEM_PROCESSOR}")
    add_compile_definitions("LINUX")

elseif(${Windows})
    message(STATUS "OS: Windows")
    message(STATUS "ARCH: ${CMAKE_SYSTEM_PROCESSOR}")
    add_compile_definitions("WIN32")

    include_directories(glib
            "src/glib/"
            "extdeps/"
            "${GLEW_LIB}/include"
            "${GLFW_LIB}/include"
    )

    find_package(OpenGL REQUIRED)

    target_link_libraries(glib
        "${CMAKE_SOURCE_DIR}/extdeps/glfw/lib-vc2022/glfw3.lib"
        opengl32
        user32
        gdi32
        shell32
    )
endif()