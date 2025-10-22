cmake_minimum_required(VERSION 3.28)
project(GraphicsAPI)

include_directories("${PROJECT_SOURCE_DIR}")

add_library(GraphicsAPI SHARED
        OpenGL/OpenGL.cpp
)