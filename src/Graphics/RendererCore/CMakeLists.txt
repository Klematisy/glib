cmake_minimum_required(VERSION 3.28)
project(RendererCore)

set(Apple   ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
set(Linux   ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
set(Windows ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

set(CMAKE_CXX_STANDARD 20)

add_library(RendererCore OBJECT
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

        element_buffer.h
        frame_buffer.h
        render_buffer.h
        vertex_array.h
        vertex_array_layout.h
        vertex_buffer.h
        preprocessor.h
        shader.h
        shader_program.h
        texture.h
        texture2D.h
        texture_array.h
        renderer.h
        window.h
)