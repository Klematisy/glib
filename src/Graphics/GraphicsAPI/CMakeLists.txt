cmake_minimum_required(VERSION 3.28)
project(GraphicsAPI)

find_package(OpenGL REQUIRED)

if (${Apple})
    add_library(GraphicsAPI SHARED
            OpenGL.cpp
            "${CMAKE_SOURCE_DIR}/extdeps/glew/src/glew.c"
    )

    target_compile_options(GraphicsAPI PRIVATE -w)

    target_link_libraries(GraphicsAPI
            "-framework Cocoa"
            "-framework IOKit"
            OpenGL::GL
    )
elseif(${Linux})
    add_library(GraphicsAPI SHARED
            OpenGL.cpp
            "${CMAKE_SOURCE_DIR}/extdeps/glew/src/glew.c"
    )

    target_compile_options(GraphicsAPI PRIVATE -w)

    target_link_libraries(GraphicsAPI
            "-framework Cocoa"
            "-framework IOKit"
            OpenGL::GL
    )
elseif(${Windows})
    add_library(GraphicsAPI STATIC
            OpenGL.cpp
            "${CMAKE_SOURCE_DIR}/extdeps/glew/src/glew.c"
    )

    target_compile_options(GraphicsAPI PRIVATE /w)

    target_link_libraries(GraphicsAPI
            opengl32
            user32
            gdi32
            shell32
    )
endif()