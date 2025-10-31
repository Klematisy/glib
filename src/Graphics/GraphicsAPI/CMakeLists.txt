cmake_minimum_required(VERSION 3.28)
project(GraphicsAPI)

find_package(OpenGL REQUIRED)

add_library(GraphicsAPI SHARED
        OpenGL.cpp
        "${CMAKE_SOURCE_DIR}/extdeps/glew/src/glew.c"
)

target_link_libraries(GraphicsAPI OpenGL::GL)