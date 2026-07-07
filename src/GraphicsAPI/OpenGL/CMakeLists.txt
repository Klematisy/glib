cmake_minimum_required(VERSION 3.28)
project(GraphicsAPI)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

set(GLEW_INCLUDE_DIR    "${CMAKE_SOURCE_DIR}/extdeps/glew/include")
set(GLEW_IMPLEMENTATION "${CMAKE_SOURCE_DIR}/extdeps/glew/src/glew.c")

set(WINDOW_IMPLEMENTATION "")
if (${WINDOW_LIBRARY_NAME} STREQUAL "GLFW")
    set(WINDOW_IMPLEMENTATION "glfw_window.cpp")
elseif(${WINDOW_LIBRARY_NAME} STREQUAL "SDL2")
    set(WINDOW_IMPLEMENTATION "sld2_window.cpp")
endif()

if (${WINDOW_IMPLEMENTATION} STREQUAL "")
    message(FATAL_ERROR "The window library not found!")
endif()

if (${GLEW_INCLUDE_DIR} STREQUAL "")
    message(FATAL_ERROR "The glew include dir not found!")
endif()

if (${GLEW_IMPLEMENTATION} STREQUAL "")
    message(FATAL_ERROR "The glew implementation not found!")
endif()

add_library(GraphicsAPI STATIC
    buffers.cpp
    renderer.cpp
    shader.cpp
    texture.cpp
    preprocessor.cpp
    graphics_api_opengl.cpp
    ${GLEW_IMPLEMENTATION}
    ${WINDOW_IMPLEMENTATION}
    ${LOGGER_LIBRARY}
)

target_compile_definitions(GraphicsAPI PRIVATE GL_LOGGER_EXISTS)
target_compile_definitions(GraphicsAPI PRIVATE __VLADLIB_GLFW__)

# if(TARGET Logger)
#     target_link_libraries(GraphicsAPI PRIVATE Logger)
#     target_compile_definitions(GraphicsAPI PRIVATE GL_LOGGER_EXISTS)
# elseif(LOGGER_INCLUDE_DIR AND LOGGER_LIBRARY)
#     target_include_directories(GraphicsAPI PRIVATE
#         ${LOGGER_INCLUDE_DIR})
#     target_link_libraries(GraphicsAPI PRIVATE
#         ${LOGGER_LIBRARY})
#     target_compile_definitions(GraphicsAPI PRIVATE GL_LOGGER_EXISTS)
# endif()

target_include_directories(GraphicsAPI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${WINDOW_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${LOGGER_INCLUDE_DIR}
)


if (${Apple})
    target_compile_options(GraphicsAPI PRIVATE -w)

    target_link_libraries(GraphicsAPI
        "-framework Cocoa"
        "-framework IOKit"
        ${WINDOW_LIBRARY}
        OpenGL::GL
    )
elseif(${Linux})
    target_compile_options(GraphicsAPI PRIVATE -w)

    target_link_libraries(GraphicsAPI
        "-framework Cocoa"
        "-framework IOKit"
        OpenGL::GL
    )
elseif(${Windows})
    target_compile_options(GraphicsAPI PRIVATE /w)

    target_link_libraries(GraphicsAPI
        opengl32
        user32
        gdi32
        shell32
    )
endif()
