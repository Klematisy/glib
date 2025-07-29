cmake_minimum_required(VERSION 3.28)
project(glib)

include(ExternalProject)

set(LOAD_DEPS False  CACHE STRING "Load dependencies")
set(LINKING   STATIC CACHE STRING "Linking Type")

set(Apple   ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
set(Linux   ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
set(Windows ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

set(LIBRARY_DIR ${CMAKE_SOURCE_DIR}/deps/library)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/deps/include)

set(MacOS_GLFW_URL "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.MACOS.zip")
set(MacOS_GLEW_URL "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.zip")

set(WIN64_GLFW_URL "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip")
set(WIN32_GLFW_URL "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN32.zip")
set(WIN_GLEW_URL "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0-win32.zip")

set(CMAKE_CXX_STANDARD 20)

add_executable(glib src/main.cpp
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
        src/Fonts/stb_tt.cpp
        src/Fonts/Font.cpp
        src/Fonts/stb_iw.cpp
        src/Fonts/Language.cpp
        src/OpenGLCore/Logger.cpp
)

if (${CMAKE_BUILD_TYPE} EQUAL Release)
    add_compile_options("-w")
endif()

function(FolderReplacer SCRIPT_NAME FOLDER_NAME SOURCE OUTPUT)
    add_custom_target(copy_${FOLDER_NAME}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT}/${FOLDER_NAME}"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${SOURCE}/${FOLDER_NAME}" "${OUTPUT}/${FOLDER_NAME}"
            DEPENDS ${SCRIPT_NAME}
            COMMENT "Copying ${FOLDER_NAME}..."
    )

    add_dependencies(glib copy_${FOLDER_NAME})
endfunction()

function(FileReplacer SCRIPT_NAME FILE_NAME SOURCE OUTPUT)
    add_custom_command(
        OUTPUT  ${LIBRARY_DIR}/${FILE_NAME}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT}
        COMMAND ${CMAKE_COMMAND} -E copy "${SOURCE}/${FILE_NAME}" "${OUTPUT}"
        DEPENDS ${SCRIPT_NAME}
        COMMENT "Copying ${FILE_NAME}..."
    )

    add_custom_target(copy_${FILE_NAME} ALL
        DEPENDS ${LIBRARY_DIR}/${FILE_NAME}
    )

    add_dependencies(glib copy_${FILE_NAME})
endfunction()

function(Downloader dir NAME URL)
    ExternalProject_Add(
        ${NAME}
        URL ${URL}
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
    )

    add_dependencies(glib ${NAME})

    ExternalProject_Get_Property(${NAME} SOURCE_DIR)

    set(${dir} ${SOURCE_DIR})
    return(PROPAGATE ${dir})
endfunction()

set(glewIncludeDir "${CMAKE_SOURCE_DIR}/deps/include/GL")
set(glfwIncludeDir "${CMAKE_SOURCE_DIR}/deps/include/GLFW")
set(glfwLibraryDir "${CMAKE_SOURCE_DIR}/deps/library")

if (   NOT EXISTS ${glewIncludeDir}
    OR NOT EXISTS ${glfwIncludeDir}
    OR NOT EXISTS ${glfwLibraryDir})
    set(LOAD_DEPS True)
    message(STATUS "LOAD_DEPS = True")
else ()
    message(STATUS "LOAD_DEPS = False")
endif ()

if (${Apple})
    message(STATUS "OS: MacOS")
    message(STATUS "ARCH: ${CMAKE_SYSTEM_PROCESSOR}")

    include_directories(glib deps/include)
    find_package(OpenGL REQUIRED)

    set(glfwLibrary "libglfw3.a")
    if (LINKING STREQUAL DYNAMIC)
        set(glfwLibrary "libglfw.3.dylib")
    endif ()

    if (${LOAD_DEPS} STREQUAL True)
        set(DIR "default val")
        Downloader(DIR glfw ${MacOS_GLFW_URL})

        set(glfwLib "lib-arm64")
        if (NOT ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
            set(glfwLib "lib-x86_64")
        endif()

        FileReplacer(glfw "libglfw3.a"        ${DIR}/${glfwLib}    ${LIBRARY_DIR})
        FileReplacer(glfw "libglfw.3.dylib"   ${DIR}/${glfwLib}    ${LIBRARY_DIR})
        FileReplacer(glfw "glfw3.h"           ${DIR}/include/GLFW  ${INCLUDE_DIR}/GLFW)

        Downloader(DIR glew ${MacOS_GLEW_URL})
        FolderReplacer(glew "GL" ${DIR}/include ${INCLUDE_DIR}/)

        add_compile_definitions("MACOS")

        add_library(glfw_imported SHARED IMPORTED GLOBAL)
        set_target_properties(glfw_imported PROPERTIES
            IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/deps/library/${glfwLibrary}"
        )

        add_dependencies(glfw_imported "copy_${glfwLibrary}")

        target_link_libraries(glib
            glfw_imported
            "-framework Cocoa"
            "-framework IOKit"
            OpenGL::GL
        )
    else ()
        target_link_libraries(glib
            "${CMAKE_SOURCE_DIR}/deps/library/${glfwLibrary}"
            "-framework Cocoa"
            "-framework IOKit"
            OpenGL::GL
        )
    endif ()

elseif(${Linux})
    message("OS: Linux")
    message(STATUS "ARCH: ${CMAKE_SYSTEM_PROCESSOR}")

elseif(${Windows})
    message(STATUS "OS: Windows")
    message(STATUS "ARCH: ${CMAKE_SYSTEM_PROCESSOR}")

    include_directories(glib deps/include)
    find_package(OpenGL REQUIRED)

    set(glfwLibrary "glfw3.lib")
    if (LINKING STREQUAL DYNAMIC)
        set(glfwLibrary "glfw3.dll")
    endif ()

    if (${LOAD_DEPS} STREQUAL True)
        set(DIR "default val")

        if (${CMAKE_SIZEOF_VOID_P} EQUAL 4)
            Downloader(DIR glfw ${WIN32_GLFW_URL})
        elseif(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
            Downloader(DIR glfw ${WIN64_GLFW_URL})
        endif ()

        FileReplacer(glfw "glfw3.lib"  ${DIR}/lib-vc2022     ${LIBRARY_DIR})
        FileReplacer(glfw "glfw3.dll"  ${DIR}/lib-vc2022     ${LIBRARY_DIR})
        FileReplacer(glfw "glfw3.h"    ${DIR}/include/GLFW   ${INCLUDE_DIR}/GLFW)

        Downloader(DIR glew ${WIN_GLEW_URL})
        FolderReplacer(glew "GL" ${DIR}/include ${INCLUDE_DIR}/)

        add_compile_definitions("WIN32")

        add_library(glfw_imported SHARED IMPORTED GLOBAL)
        set_target_properties(glfw_imported PROPERTIES
            IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/deps/library/${glfwLibrary}"
        )

        add_dependencies(glfw_imported "copy_${glfwLibrary}")

        target_link_libraries(glib
            glfw_imported
            opengl32
            user32
            gdi32
            shell32)
    else()
        target_link_libraries(glib
            "${CMAKE_SOURCE_DIR}/deps/library/${glfwLibrary}"
            opengl32
            user32
            gdi32
            shell32
        )
    endif ()
endif()