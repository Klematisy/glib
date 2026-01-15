cmake_minimum_required(VERSION 3.28)
project(Geometry)

add_library(Geometry OBJECT
        mesh.cpp
        text.cpp
        entity.cpp
        camera.cpp

        mesh.h
        text.h
        entity.h
        transform.h
        material.h
        camera.h
)