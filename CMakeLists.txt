cmake_minimum_required(VERSION 3.28)
project(installer)
project(glib)

add_custom_target(installer
                  COMMENT "Installer picked")

include(ExternalProject)

set(LOAD_DEPS True   CACHE STRING "Load dependencies")
set(LINKING   STATIC CACHE STRING "Linking Type")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

set(Apple   ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
set(Linux   ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
set(Windows ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

set(OpenGLCoreDir "${CMAKE_SOURCE_DIR}/src/glib/OpenGLCore")

set(LIBRARY_DIR ${CMAKE_SOURCE_DIR}/extdeps/library)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extdeps/include)

set(GLEW_DIR "Dir" CACHE INTERNAL "The downloaded glew path")

set(MacOS_GLFW_URL "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.MACOS.zip")
set(MacOS_GLEW_URL "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.zip")

set(WIN64_GLFW_URL "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip")
set(WIN32_GLFW_URL "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN32.zip")
set(WIN_GLEW_URL   "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0-win32.zip")

set(CMAKE_CXX_STANDARD 20)

add_executable(glib src/main.cpp
        src/glib/Base/window.cpp
        src/glib/Base/draw.cpp
        src/glib/Base/batch.cpp
        src/glib/Base/create_shape.cpp
        src/glib/Base/texture_manager.cpp
        src/glib/Base/camera.cpp
        src/glib/Base/shader.cpp
        src/glib/Fonts/stb_tt.cpp
        src/glib/Fonts/font.cpp
        src/glib/Fonts/stb_iw.cpp
        src/glib/Fonts/language.cpp
        src/glib/Base/texutre.cpp
        src/glib/Utils/atlas.cpp
)





message(STATUS ${CMAKE_BUILD_TYPE})
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(glib PRIVATE
            $<$<CXX_COMPILER_ID:GNU,Clang>:-w>
            $<$<CXX_COMPILER_ID:MSVC>:/W0>
    )

    message(STATUS "Warnings turned off")
    add_compile_definitions("__GLIB_RELEASE__")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions("__GLIB_DEBUG__")
endif()




function(FolderReplacer SCRIPT_NAME FOLDER_NAME SOURCE OUTPUT)
    add_custom_target(copy_${FOLDER_NAME}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT}/${FOLDER_NAME}"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${SOURCE}/${FOLDER_NAME}" "${OUTPUT}/${FOLDER_NAME}"
            DEPENDS ${SCRIPT_NAME}
            COMMENT "Copying ${FOLDER_NAME}..."
    )

    add_dependencies(installer copy_${FOLDER_NAME})
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

    add_dependencies(installer copy_${FILE_NAME})
endfunction()

function(Downloader dir NAME URL)
    ExternalProject_Add(
            ${NAME}
            URL ${URL}
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ""
    )

    add_dependencies(installer ${NAME})

    ExternalProject_Get_Property(${NAME} SOURCE_DIR)

    set(${dir} ${SOURCE_DIR})
    return(PROPAGATE ${dir})
endfunction()

set(glewIncludeDir "${CMAKE_SOURCE_DIR}/extdeps/include/GL")
set(glfwIncludeDir "${CMAKE_SOURCE_DIR}/extdeps/include/GLFW")
set(glfwLibraryDir "${CMAKE_SOURCE_DIR}/extdeps/library")




function(install_apple)
    set(glfwLibrary "libglfw3.a")
    if (LINKING STREQUAL DYNAMIC)
        set(glfwLibrary "libglfw.3.dylib")
    endif ()

    add_library(glfw_imported SHARED IMPORTED GLOBAL)
    set_target_properties(glfw_imported PROPERTIES
            IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/extdeps/library/${glfwLibrary}"
    )

    add_dependencies(glfw_imported "copy_${glfwLibrary}")

    set(DIR "default val")
    Downloader(DIR glfw ${MacOS_GLFW_URL})

    set(glfwLib "lib-arm64")
    if (NOT ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
        set(glfwLib "lib-x86_64")
    endif()

    FileReplacer(glfw "libglfw3.a"        ${DIR}/${glfwLib}    ${LIBRARY_DIR})
    FileReplacer(glfw "libglfw.3.dylib"   ${DIR}/${glfwLib}    ${LIBRARY_DIR})
    FileReplacer(glfw "glfw3.h"           ${DIR}/include/GLFW  ${INCLUDE_DIR}/GLFW)

    Downloader(GLEW_DIR glew ${MacOS_GLEW_URL})
    FolderReplacer(glew "GL" ${GLEW_DIR}/include ${INCLUDE_DIR}/)

    add_subdirectory(${OpenGLCoreDir})
    add_dependencies(installer OpenGLCore_install)
endfunction()






if (${Apple})
    message(STATUS "OS: MacOS")
    message(STATUS "ARCH: ${CMAKE_SYSTEM_PROCESSOR}")

    include_directories(glib extdeps/include
                             src/glib/
    )

    find_package(OpenGL REQUIRED)

    install_apple()

    add_compile_definitions("MACOS")

    add_dependencies(glib OpenGLCore)

    target_link_libraries(glib
            "${CMAKE_SOURCE_DIR}/extdeps/library/libglfw3.a"
            "-framework Cocoa"
            "-framework IOKit"
            OpenGL::GL
            OpenGLCore
    )

elseif(${Linux})
    message("OS: Linux")
    message(STATUS "ARCH: ${CMAKE_SYSTEM_PROCESSOR}")



elseif(${Windows})
#    message(STATUS "OS: Windows")
#    message(STATUS "ARCH: ${CMAKE_SYSTEM_PROCESSOR}")
#
#    include_directories(glib extdeps/include)
#    find_package(OpenGL REQUIRED)
#
#    set(glfwLibrary "glfw3.lib")
#    if (LINKING STREQUAL DYNAMIC)
#        set(glfwLibrary "glfw3.dll")
#    endif ()
#
#    if (${LOAD_DEPS} STREQUAL True)
#        set(DIR "default val")
#
#        if (${CMAKE_SIZEOF_VOID_P} EQUAL 4)
#            Downloader(DIR glfw ${WIN32_GLFW_URL})
#        elseif(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
#            Downloader(DIR glfw ${WIN64_GLFW_URL})
#        endif ()
#
#        FileReplacer(glfw "glfw3.lib"  ${DIR}/lib-vc2022     ${LIBRARY_DIR})
#        FileReplacer(glfw "glfw3.dll"  ${DIR}/lib-vc2022     ${LIBRARY_DIR})
#        FileReplacer(glfw "glfw3.h"    ${DIR}/include/GLFW   ${INCLUDE_DIR}/GLFW)
#
#        Downloader(DIR glew ${WIN_GLEW_URL})
#        FolderReplacer(glew "GL" ${DIR}/include ${INCLUDE_DIR}/)
#
#        add_compile_definitions("WIN32")
#
#        add_library(glfw_imported SHARED IMPORTED GLOBAL
#                src/texutre.cpp)
#        set_target_properties(glfw_imported PROPERTIES
#                IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/extdeps/library/${glfwLibrary}"
#        )
#
#        add_dependencies(glfw_imported "copy_${glfwLibrary}")
#
#        target_link_libraries(glib
#                glfw_imported
#                opengl32
#                user32
#                gdi32
#                shell32)
#    else()
#        target_link_libraries(glib
#                "${CMAKE_SOURCE_DIR}/extdeps/library/${glfwLibrary}"
#                opengl32
#                user32
#                gdi32
#                shell32
#        )
#    endif ()
endif()