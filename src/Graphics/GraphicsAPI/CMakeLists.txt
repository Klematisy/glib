cmake_minimum_required(VERSION 3.28)
project(GraphicsAPI)

set(CMAKE_CXX_STANDARD 20)
include_directories(${CMAKE_SOURCE_DIR})

find_package(OpenGL REQUIRED)

if (${Apple})
    add_library(GraphicsAPI STATIC
        opengl.cpp
        graphics_api.h
        graphics_api_opengl.h
        opengl_glfw_context.cpp
        "${CMAKE_SOURCE_DIR}/extdeps/glew/src/glew.c"
    )

    target_compile_options(GraphicsAPI PRIVATE -w)

    target_link_libraries(GraphicsAPI
        "-framework Cocoa"
        "-framework IOKit"
        OpenGL::GL
    )
elseif(${Linux})
    add_library(GraphicsAPI STATIC
        opengl.cpp
        graphics_api.h
        graphics_api_opengl.h
        opengl_glfw_context.cpp
        "${CMAKE_SOURCE_DIR}/extdeps/glew/src/glew.c"
    )

    target_compile_options(GraphicsAPI PRIVATE -w)

    target_link_libraries(GraphicsAPI
        "-framework Cocoa"
        "-framework IOKit"
        OpenGL::GL
    )
elseif(${Windows})
    add_library(GraphicsAPI OBJECT
        opengl.cpp
        graphics_api.h
        graphics_api_opengl.h
        opengl_glfw_context.cpp
        "${CMAKE_SOURCE_DIR}/extdeps/glew/src/glew.c"
    )

    target_compile_options(GraphicsAPI PRIVATE /w)

    target_link_libraries(GraphicsAPI
        opengl32
        user32
        gdi32
        shell32
    )
endif()
