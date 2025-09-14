cmake_minimum_required(VERSION 3.28)
project(OpenGLCore)
project(OpenGLCore_install)

add_custom_target(OpenGLCore_install
        COMMENT "Install glew.c")

set(Apple   ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
set(Linux   ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
set(Windows ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/deps/include")


file(GLOB glew_src "glew.c")
add_library(OpenGLCore STATIC
        Buffer/element_buffer.cpp
        Buffer/vertex_buffer.cpp
        Buffer/vertex_array.cpp
        Texture/stb_image.cpp
        Texture/texture.cpp
        Texture/texture_array.cpp
        Shader/shader_program.cpp
        Shader/shader_utils.cpp
        shader_cache.cpp
        renderer.cpp
        logger.cpp
        ${glew_src}
)


function(FileReplacer SCRIPT_NAME FILE_NAME SOURCE OUTPUT)
    add_custom_command(
            OUTPUT  ${GLEW_DIR}/${FILE_NAME}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT}
            COMMAND ${CMAKE_COMMAND} -E copy "${SOURCE}/${FILE_NAME}" "${OUTPUT}"
            DEPENDS ${SCRIPT_NAME}
            COMMENT "Copying ${FILE_NAME}..."
    )

    add_custom_target(copy_${FILE_NAME} ALL
            DEPENDS ${GLEW_DIR}/${FILE_NAME}
    )

    add_dependencies(OpenGLCore_install copy_${FILE_NAME})
endfunction()

if (Apple)
    FileReplacer(glew "glew.c" "${GLEW_DIR}/src" ".")
    message(${GLEW_DIR}/src)
    target_link_libraries(OpenGLCore
            OpenGL::GL
            "-framework Cocoa"
            "-framework IOKit"
    )
elseif (Linux)

elseif (Windows)
#    FileReplacer(glew_src "glew.c" "${GLEW_DIR}/src" ".")

    target_link_libraries(
            opengl32
            user32
            gdi32
            shell32)
endif ()