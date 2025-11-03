
# BUILD_TYPE - variable for specifying the build type {Release, Debug}
# Basic build type
BUILD_TYPE ?= Release
BUILD_DEPS ?= ON
BUILD_FOLDER_NAME=r

THIS_MAKEFILE := $(lastword $(MAKEFILE_LIST))
THIS_DIR := $(dir $(realpath $(THIS_MAKEFILE)))

VENV_DIR := venv
VENV_PYTHON := $(VENV_DIR)/bin/python
VENV_PIP := $(VENV_DIR)/bin/pip

ZLIB_LIB := -DZLIB_LIBRARY=$(THIS_DIR)"extdeps/zlib/lib/libz.dylib" -DZLIB_INCLUDE_DIR=$(THIS_DIR)"extdeps/zlib"
PNG_LIB := -DPNG_LIBRARY=$(THIS_DIR)"extdeps/libpng/lib/libpng16.dylib" -DPNG_INCLUDE_DIR=$(THIS_DIR)"extdeps/libpng/include"
BZIP2_LIB := -DBZIP2_LIBRARY=$(THIS_DIR)"extdeps/brotli/lib/libbz2.dylib" -DBZIP2_INCLUDE_DIR=$(THIS_DIR)"extdeps/brotli/include"
HarfBuzz_LIB := -DHarfBuzz_LIBRARY=$(THIS_DIR)"extdeps/HarfBuzz/lib/libharfbuzz.a" -DHarfBuzz_INCLUDE_DIR=$(THIS_DIR)"extdeps/HarfBuzz/include"

MSDFGEN_DIR := extdeps/msdf-atlas-gen
FREETYPE_DIR := extdeps/freetype

ifeq ($(BUILD_TYPE), Release)
	BUILD_FOLDER_NAME:=r
else ifeq ($(BUILD_TYPE), Debug)
	BUILD_FOLDER_NAME:=d
endif

install:
	python3 -m venv venv
	$(VENV_PIP) install requests
	$(VENV_PYTHON) install_script.py

build_dependencies:
	sudo rm -rf "extdeps"
	sudo mkdir "extdeps"

	sudo cmake -S downloads/zlib -B downloads/zlib/build -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DCMAKE_INSTALL_PREFIX="extdeps/zlib"
	sudo cmake --build downloads/zlib/build --parallel
	sudo cmake --install downloads/zlib/build --config $(BUILD_TYPE)

	sudo cmake -S downloads/brotli -B downloads/brotli/build -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	sudo cmake --build downloads/brotli/build --parallel
	sudo cmake --install downloads/brotli/build --prefix extdeps/brotli --config $(BUILD_TYPE)

	sudo cmake -S downloads/libpng -B downloads/libpng/build \
	-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) $(ZLIB_LIB)

	sudo cmake --build downloads/libpng/build --parallel
	sudo cmake --install downloads/libpng/build --prefix extdeps/libpng --config $(BUILD_TYPE)

	sudo cmake -S downloads/HarfBuzz -B downloads/HarfBuzz/build -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	sudo cmake --build downloads/HarfBuzz/build --parallel
	sudo cmake --install downloads/HarfBuzz/build --prefix extdeps/HarfBuzz --config $(BUILD_TYPE)

	sudo cmake -S downloads/freetype -B downloads/freetype/build 	\
	$(ZLIB_LIB) $(PNG_LIB) $(BZIP2_LIB) $(HarfBuzz_LIB) 		 	\
	-DCMAKE_INSTALL_PREFIX=extdeps/freetype -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

	sudo cmake --build downloads/freetype/build --parallel
	sudo cmake --install downloads/freetype/build

	sudo cp -R downloads/msdf-atlas-gen extdeps/msdf-atlas-gen
	sudo cp -R downloads/stb extdeps/stb
	sudo cp -R downloads/glm extdeps/glm
	sudo cp -R downloads/glfw extdeps/glfw
	sudo cp -R downloads/glew extdeps/glew


clear_dependencies:
	sudo rm -rf downloads/brotli/build
	sudo rm -rf downloads/libpng/build
	sudo rm -rf downloads/HarfBuzz/build
	sudo rm -rf downloads/zlib/build
	sudo rm -rf downloads/freetype/build


configure:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

build:
	cmake --build build --parallel

run:
	./build/glib

clear:
	rm -rf dbuild rbuild

clearAllCache:
	sudo rm -rf dbuild rbuild venv extdeps downloads