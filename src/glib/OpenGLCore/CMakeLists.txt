cmake_minimum_required(VERSION 3.28)
project(OpenGLCore)

set(Apple   ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
set(Linux   ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
set(Windows ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/deps/include")

#message("${CMAKE_SOURCE_DIR}/..")

add_library(OpenGLCore STATIC
        glew.c
        Texture/stb_image.cpp
        Buffer/vertex_buffer.cpp
        Buffer/vertex_array.cpp
        Shader/shader_program.cpp
        Texture/texture.cpp
        renderer.cpp
        Buffer/element_buffer.cpp
        Shader/shader_utils.cpp
        Texture/texture_array.cpp
        logger.cpp
        shader_cache.cpp
)

if (Apple)
    target_link_libraries(OpenGLCore
            OpenGL::GL
            "-framework Cocoa"
            "-framework IOKit"
    )
elseif (Linux)

elseif (Windows)
    target_link_libraries(opengl32)
endif ()