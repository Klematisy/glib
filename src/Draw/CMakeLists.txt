cmake_minimum_required(VERSION 3.28)
project(Draw)

add_library(Draw OBJECT
        batch.inl
        frame_buffer.cpp
        shader.cpp
        texture.cpp
        texture_manager.cpp

        batch.h
        frame_buffer.h
        shader.h
        structs.h
        texture.h
        texture_manager.h
)