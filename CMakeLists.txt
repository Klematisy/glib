cmake_minimum_required(VERSION 3.28)
project(glib)

set(CMAKE_CXX_STANDARD 20)

set(LINKING_TYPE STATIC CACHE STRING "Linking Type")
set(BUILD_TEST "ON" CACHE STRING "Build test" FORCE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

set(Apple   "${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
set(Linux   "${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
set(Windows "${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")

set(GraphicsAPI_DIR   "${CMAKE_SOURCE_DIR}/src/Graphics/GraphicsAPI")
set(RendererDIR       "${CMAKE_SOURCE_DIR}/src/Graphics/RendererCore")
set(GeometryDIR       "${CMAKE_SOURCE_DIR}/src/Geometry")
set(FontGeneratorDIR  "${CMAKE_SOURCE_DIR}/src/FontGenerator")
set(LoggerDIR         "${CMAKE_SOURCE_DIR}/src/Logger")
set(UtilsDIR           "${CMAKE_SOURCE_DIR}/src/Utils")
set(DrawDIR           "${CMAKE_SOURCE_DIR}/src/Draw")

set(GLEW_LIB           "${CMAKE_SOURCE_DIR}/extdeps/glew" CACHE STRING "GLEW path")
set(GLFW_LIB           "${CMAKE_SOURCE_DIR}/extdeps/glfw")
set(GLM_LIB            "${CMAKE_SOURCE_DIR}/extdeps/glm")
set(STB_LIB            "${CMAKE_SOURCE_DIR}/extdeps/stb")
set(MSDFGEN_ATLAS_LIB  "${CMAKE_SOURCE_DIR}/extdeps/msdf-atlas-gen")

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

add_library(glib)
if (${LINKING_TYPE} STREQUAL STATIC)
    add_library(glib STATIC)
elseif (${LINKING_TYPE} STREQUAL DYNAMIC)
    add_library(glib SHARED)
endif ()

add_subdirectory(${GraphicsAPI_DIR})
add_subdirectory(${FontGeneratorDIR})
add_subdirectory(${RendererDIR})
add_subdirectory(${GeometryDIR})
add_subdirectory(${LoggerDIR})
add_subdirectory(${UtilsDIR})
add_subdirectory(${DrawDIR})

add_dependencies(glib GraphicsAPI)
add_dependencies(glib FontGenerator)
add_dependencies(glib RendererCore)
add_dependencies(glib Geometry)
add_dependencies(glib Logger)
add_dependencies(glib Utils)
add_dependencies(glib Draw)



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


if ("${BROTLIDEC_LIBRARIES}" STREQUAL "")
    find_package(brotlidec REQUIRED)
endif ()

if ("${BZIP2_LIBRARY}" STREQUAL "")
    find_package(bzip2 REQUIRED)
endif ()

if ("${FREETYPE_LIBRARY}" STREQUAL "")
    find_package(freetype REQUIRED)
endif ()

if ("${ZLIB_LIBRARY}" STREQUAL "")
    find_package(zlib REQUIRED)
endif ()

if ("${PNG_LIBRARY}" STREQUAL "")
    find_package(png REQUIRED)
endif ()


set(LINK_LIBRARIES
    ${MSDF_ATLAS_GEN_LIBRARY}
    ${MSDFGEN_LIBRARIES}
    ${BROTLIDEC_LIBRARIES}
    ${BROTLICOMMON_LIBRARY}
    ${BZIP2_LIBRARY}
    ${FREETYPE_LIBRARY}
    ${ZLIB_LIBRARY}
    ${PNG_LIBRARY}

    GraphicsAPI
    FontGenerator
    RendererCore
    Geometry
    Logger
    Utils
    Draw
)


if (${Apple})
    message(STATUS "OS: MacOS")
    message(STATUS "ARCH: ${CMAKE_SYSTEM_PROCESSOR}")
    add_compile_definitions("MACOS")

    set(GLFW_LIB_DIR "")
    if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/extdeps/glfw/lib-x86_64/libglfw3.a")
    elseif (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
        set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/extdeps/glfw/lib-arm64/libglfw3.a")
    endif()

    target_link_libraries(glib
        "-framework Cocoa"
        "-framework IOKit"
        "${GLFW_LIB_DIR}"
        "${LINK_LIBRARIES}"
    )
elseif(${Linux})
    message("OS: Linux")
    message(STATUS "ARCH: ${CMAKE_SYSTEM_PROCESSOR}")
    add_compile_definitions("LINUX")

elseif(${Windows})
    message(STATUS "OS: Windows")
    message(STATUS "ARCH: ${CMAKE_SYSTEM_PROCESSOR}")
    add_compile_definitions("WIN32")

    target_link_libraries(glib
        "${CMAKE_SOURCE_DIR}/extdeps/glfw/lib-vc2022/glfw3.lib"
        "${LINK_LIBRARIES}"
    )
endif()


if (${BUILD_TEST})
    project(GLIB_TEST)
    add_executable(GLIB_TEST src/Test/main.cpp)
    target_link_libraries(GLIB_TEST glib)
endif ()