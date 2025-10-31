cmake_minimum_required(VERSION 3.28)
project(RendererCore)

set(Apple   ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
set(Linux   ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
set(Windows ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

set(CMAKE_CXX_STANDARD 20)

add_library(RendererCore STATIC
        Buffer/element_buffer.cpp
        Buffer/vertex_buffer.cpp
        Buffer/vertex_array.cpp
        Buffer/frame_buffer.cpp
        Buffer/render_buffer.cpp
        Texture/stb_image.cpp
        Texture/texture2D.cpp
        Texture/texture_array.cpp
        Shader/preprocessor.cpp
        Shader/shader_program.cpp
        Shader/shader.cpp
        renderer.cpp
        window.cpp
        "${GLEW_LIB}/src/glew.c"
)

if (Apple)
    target_link_libraries(RendererCore
            OpenGL::GL
            "-framework Cocoa"
            "-framework IOKit"
    )
elseif (Linux)

elseif (Windows)
    target_link_libraries(RendererCore
            opengl32
            user32
            gdi32
            shell32)
endif ()