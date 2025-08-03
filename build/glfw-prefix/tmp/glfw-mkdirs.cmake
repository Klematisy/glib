# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glfw-prefix/src/glfw")
  file(MAKE_DIRECTORY "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glfw-prefix/src/glfw")
endif()
file(MAKE_DIRECTORY
  "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glfw-prefix/src/glfw-build"
  "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glfw-prefix"
  "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glfw-prefix/tmp"
  "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glfw-prefix/src/glfw-stamp"
  "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glfw-prefix/src"
  "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glfw-prefix/src/glfw-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glfw-prefix/src/glfw-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glfw-prefix/src/glfw-stamp${cfgdir}") # cfgdir has leading slash
endif()
