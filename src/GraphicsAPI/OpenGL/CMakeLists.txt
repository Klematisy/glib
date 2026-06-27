cmake_minimum_required(VERSION 3.28)
project(GraphicsAPI)

set(CMAKE_CXX_STANDARD 20)
include_directories(${CMAKE_SOURCE_DIR})

find_package(OpenGL REQUIRED)

if (${Apple})
    add_library(GraphicsAPI STATIC
        buffers.cpp
        renderer.cpp
        shader.cpp
        texture.cpp
        glfw_window.cpp
        graphics_api_opengl.cpp
        preprocessor.cpp
        "${CMAKE_SOURCE_DIR}/extdeps/glew/src/glew.c"
    )

    target_compile_options(GraphicsAPI PRIVATE -w)

    target_link_libraries(GraphicsAPI
        "-framework Cocoa"
        "-framework IOKit"
        OpenGL::GL
        vladlib
    )
elseif(${Linux})

    target_compile_options(GraphicsAPI PRIVATE -w)

    target_link_libraries(GraphicsAPI
        "-framework Cocoa"
        "-framework IOKit"
        OpenGL::GL
    )
elseif(${Windows})
    add_library(GraphicsAPI STATIC
        buffers.cpp
        renderer.cpp
        shader.cpp
        texture.cpp
        glfw_window.cpp
        graphics_api_opengl.cpp
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
