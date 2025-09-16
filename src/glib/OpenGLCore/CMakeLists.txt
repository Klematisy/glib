cmake_minimum_required(VERSION 3.28)
project(OpenGLCore)
project(OpenGLCore_install)

add_custom_target(OpenGLCore_install
        COMMENT "Install glew.c")

set(Apple   ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
set(Linux   ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
set(Windows ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/deps/include")


add_library(OpenGLCore STATIC
        Buffer/element_buffer.cpp
        Buffer/vertex_buffer.cpp
        Buffer/vertex_array.cpp
        Texture/stb_image.cpp
        Texture/texture.cpp
        Texture/texture_array.cpp
        Shader/shader_program.cpp
        Shader/shader_utils.cpp
        shader_cache.cpp
        renderer.cpp
        logger.cpp
        "${GLEW_LIB}/src/glew.c"
)


if (Apple)
    target_link_libraries(OpenGLCore
            OpenGL::GL
            "-framework Cocoa"
            "-framework IOKit"
    )
elseif (Linux)

elseif (Windows)
    target_link_libraries(
            opengl32
            user32
            gdi32
            shell32)
endif ()