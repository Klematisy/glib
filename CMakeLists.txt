cmake_minimum_required(VERSION 3.28)
project(glib)

set(LOAD_DEPS True   CACHE STRING "Load dependencies")
set(LINKING   STATIC CACHE STRING "Linking Type")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

set(Apple   ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
set(Linux   ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
set(Windows ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

set(OpenGLCoreDir "${CMAKE_SOURCE_DIR}/src/glib/OpenGLCore")
set(LoggerDir "${CMAKE_SOURCE_DIR}/src/glib/Logger")
set(DrawDir "${CMAKE_SOURCE_DIR}/src/glib/Draw")
set(CMAKE_CXX_STANDARD 20)

set(GLEW_LIB "${CMAKE_SOURCE_DIR}/extdeps/glew" CACHE STRING "GLEW path")
set(GLFW_LIB "${CMAKE_SOURCE_DIR}/extdeps/glfw")
set(GLM_LIB  "${CMAKE_SOURCE_DIR}/extdeps/glm")
set(STB_LIB  "${CMAKE_SOURCE_DIR}/extdeps/stb")


add_executable(glib src/main.cpp
        src/glib/Fonts/stb_tt.cpp
        src/glib/Fonts/font.cpp
        src/glib/Fonts/stb_iw.cpp
        src/glib/Fonts/language.cpp
        src/glib/Utils/camera.cpp
        src/glib/Utils/atlas.cpp
        src/glib/Geometry/mesh.cpp
        "${GLEW_LIB}/src/glew.c"
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

set(GLFW_LIB_DIR "")
if (${Apple})
    message(STATUS "OS: MacOS")
    message(STATUS "ARCH: ${CMAKE_SYSTEM_PROCESSOR}")
    add_compile_definitions("MACOS")

    include_directories(glib
            "src/glib/"
            "extdeps/"
            "${GLM_LIB}"
            "${STB_LIB}"
            "${GLEW_LIB}/include"
            "${GLFW_LIB}/include"
    )

    find_package(OpenGL REQUIRED)

    add_subdirectory(${OpenGLCoreDir})
    add_subdirectory(${LoggerDir})
    add_subdirectory(${DrawDir})

    add_dependencies(glib OpenGLCore)
    add_dependencies(glib logger)
    add_dependencies(glib Draw)

    if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/extdeps/glfw/lib-x86_64/libglfw3.a")
    elseif (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
        set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/extdeps/glfw/lib-arm64/libglfw3.a")
    endif ()


    target_link_libraries(glib
        "${GLFW_LIB_DIR}"
        "-framework Cocoa"
        "-framework IOKit"
        OpenGL::GL
        OpenGLCore
        logger
        Draw
    )
elseif(${Linux})
    message("OS: Linux")
    message(STATUS "ARCH: ${CMAKE_SYSTEM_PROCESSOR}")

elseif(${Windows})
    message(STATUS "OS: Windows")
    message(STATUS "ARCH: ${CMAKE_SYSTEM_PROCESSOR}")
    add_compile_definitions("win32")

    include_directories(glib
            "src/glib/"
            "extdeps/"
            "${GLM_LIB}"
            "${STB_LIB}"
            "${GLEW_LIB}/include"
            "${GLFW_LIB}/include"
    )

    set(CMAKE_PREFIX_PATH ${GLEW_LIB})

    find_package(OpenGL REQUIRED)

    add_subdirectory(${OpenGLCoreDir})
    add_subdirectory(${LoggerDir})
    add_subdirectory(${DrawDir})

    add_dependencies(glib OpenGLCore)
    add_dependencies(glib logger)
    add_dependencies(glib Draw)

    target_link_libraries(glib
        "${CMAKE_SOURCE_DIR}/extdeps/glfw/lib-vc2022/glfw3.lib"
        opengl32
        user32
        gdi32
        shell32
        OpenGLCore
        logger
        Draw
    )
endif()