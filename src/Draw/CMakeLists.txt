cmake_minimum_required(VERSION 3.28)
project(Draw)

add_library(Draw STATIC
        batch.cpp
        drawer.cpp
        shader.cpp
        texture.cpp
        texture_manager.cpp
)