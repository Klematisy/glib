cmake_minimum_required(VERSION 3.28)
project(FontGenerator)

add_library(FontGenerator OBJECT
        font.cpp
        font_tile.cpp
        language_tile_set.cpp
        stb_iw.cpp
)