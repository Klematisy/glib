cmake_minimum_required(VERSION 3.28)
project(oopGl)

set(CMAKE_CXX_STANDARD 17)

include_directories(oopGl deps/include)

find_package(OpenGL REQUIRED)

add_executable(oopGl src/main.cpp
        src/OpenGLCore/glew.c
        src/OpenGLCore/Texture/stb_image.cpp
        src/OpenGLCore/Buffer/VertexBuffer.cpp
        src/OpenGLCore/Buffer/VertexArray.cpp
        src/OpenGLCore/Shader/ShaderProgram.cpp
        src/OpenGLCore/Texture/Texture.cpp
        src/OpenGLCore/Renderer.cpp
        src/OpenGLCore/Buffer/ElementBuffer.cpp
        src/Window.cpp
        src/OpenGLCore/Shader/ShaderUtils.cpp
        src/glib.cpp
        src/batch.cpp
        src/CreateShape.cpp
        src/TextureSlotManager.cpp
        src/Camera.cpp
        src/Shader.cpp
        src/OpenGLCore/ShaderCache.cpp
)

target_link_libraries(oopGl "${CMAKE_SOURCE_DIR}/deps/library/libglfw.3.4.dylib")
target_link_libraries(oopGl OpenGL::GL)