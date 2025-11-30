cmake_minimum_required(VERSION 3.28)
project(Draw)

add_library(Draw OBJECT
        batch.cpp
        drawer.cpp
        buffer_drawer.cpp
        frame_buffer.cpp
        shader.cpp
        texture.cpp
        texture_manager.cpp
)