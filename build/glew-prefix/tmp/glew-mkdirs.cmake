# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glew-prefix/src/glew")
  file(MAKE_DIRECTORY "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glew-prefix/src/glew")
endif()
file(MAKE_DIRECTORY
  "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glew-prefix/src/glew-build"
  "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glew-prefix"
  "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glew-prefix/tmp"
  "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glew-prefix/src/glew-stamp"
  "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glew-prefix/src"
  "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glew-prefix/src/glew-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glew-prefix/src/glew-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/agucs/Desktop/main/cpp/OpenGL/oopGl/build/glew-prefix/src/glew-stamp${cfgdir}") # cfgdir has leading slash
endif()
