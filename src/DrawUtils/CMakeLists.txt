cmake_minimum_required(VERSION 3.28)
project(DrawUtils)

add_library(DrawUtils OBJECT
        batch.inl
        frame_buffer.cpp
        shader.cpp
        texture_manager.cpp

        batch.h
        frame_buffer.h
        shader.h
        structs.h
        texture_manager.h
)