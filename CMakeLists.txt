cmake_minimum_required(VERSION 3.28)
project(vladlib)

set(CMAKE_CXX_STANDARD 20)

set(LINKING_TYPE STATIC CACHE STRING "Linking Type")
set(BUILD_TEST "ON" CACHE STRING "Build test" FORCE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

set(Apple   "${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
set(Linux   "${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
set(Windows "${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")

set(GraphicsAPI_DIR   "${CMAKE_SOURCE_DIR}/src/GraphicsAPI")
set(GeometryDIR       "${CMAKE_SOURCE_DIR}/src/Geometry")
set(FontGeneratorDIR  "${CMAKE_SOURCE_DIR}/src/FontGenerator")
set(LoggerDIR         "${CMAKE_SOURCE_DIR}/src/Logger")
set(DrawDIR           "${CMAKE_SOURCE_DIR}/src/DrawUtils")

set(GLEW_LIB           "${CMAKE_SOURCE_DIR}/extdeps/glew" CACHE STRING "GLEW path")
set(GLFW_LIB           "${CMAKE_SOURCE_DIR}/extdeps/glfw")
set(GLM_LIB            "${CMAKE_SOURCE_DIR}/extdeps/glm")
set(STB_LIB            "${CMAKE_SOURCE_DIR}/extdeps/stb")
set(MSDFGEN_ATLAS_LIB  "${CMAKE_SOURCE_DIR}/extdeps/msdf-atlas-gen")

set(RENDERER_BACKEND "OPENGL")
set(WINDOW_LIBRARY_NAME "GLFW")
set(WINDOW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/extdeps/glfw/include")
set(WINDOW_LIBRARY "${CMAKE_SOURCE_DIR}/extdeps/glfw/lib-arm64/libglfw3.a")

include_directories(vladlib
    "src/"
    "extdeps/"
    "${GLM_LIB}"
    "${STB_LIB}"
    "${MSDFGEN_ATLAS_LIB}"
    "${MSDFGEN_ATLAS_LIB}/msdfgen"
)

add_library(vladlib ${LINKING_TYPE})

add_subdirectory(${GraphicsAPI_DIR})
add_subdirectory(${FontGeneratorDIR})
add_subdirectory(${GeometryDIR})
add_subdirectory(${LoggerDIR})
add_subdirectory(${DrawDIR})

add_dependencies(vladlib GraphicsAPI)
add_dependencies(vladlib FontGenerator)
add_dependencies(vladlib Geometry)
add_dependencies(vladlib Logger)
add_dependencies(vladlib DrawUtils)

message(STATUS ${CMAKE_BUILD_TYPE})
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(vladlib PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-w>
        $<$<CXX_COMPILER_ID:MSVC>:/W0>
    )

#    TODO: обмыслить момент с vla
   # target_compile_options(my_target PRIVATE
   #     -Wvla-extension
   #     -Werror=vla-extension
   # )

    message(STATUS "Warnings turned off")
    target_compile_definitions(vladlib PUBLIC "__VLADLIB_RELEASE__")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(vladlib PUBLIC "__VLADLIB_DEBUG__")
endif()

if ("${FREETYPE_LIBRARY}" STREQUAL "")
    find_package(freetype REQUIRED)
endif ()

set(LINK_LIBRARIES
    ${MSDF_ATLAS_GEN_LIBRARY}
    ${MSDFGEN_LIBRARIES}
    ${FREETYPE_LIBRARY}

    GraphicsAPI
    FontGenerator
    Geometry
    Logger
    DrawUtils
)


if (${Apple})
    message(STATUS "OS: MacOS")
    message(STATUS "ARCH: ${CMAKE_OSX_ARCHITECTURES}")
    add_compile_definitions("MACOS")

    target_link_libraries(vladlib PUBLIC
        "-framework Cocoa"
        "-framework IOKit"
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

    target_link_libraries(vladlib
        "${CMAKE_SOURCE_DIR}/extdeps/glfw/lib-vc2022/glfw3.lib"
        "${LINK_LIBRARIES}"
    )
endif()


if (${BUILD_TEST})
    project(VLADLIB_TEST)
    add_executable(VLADLIB_TEST src/Test/main.cpp)
    target_link_libraries(VLADLIB_TEST vladlib)
endif ()
