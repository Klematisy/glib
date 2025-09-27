cmake_minimum_required(VERSION 3.28)
project(Draw)

add_library(Draw STATIC
        batch.cpp
        create_shape.cpp
        drawer.cpp
        shader.cpp
        texture_manager.cpp
        texture.cpp
)