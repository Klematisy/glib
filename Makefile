
# BUILD_TYPE - variable for specifying the build type {Release, Debug}
# Basic build type
BUILD_TYPE ?= Release
BUILD_DEPS ?= ON

THIS_MAKEFILE := $(lastword $(MAKEFILE_LIST))
THIS_DIR := $(dir $(realpath $(THIS_MAKEFILE)))

VENV_DIR := venv
VENV_PYTHON := $(VENV_DIR)/bin/python
VENV_PIP := $(VENV_DIR)/bin/pip

BZIP2_LIB_TYPE := -DBZIP2_LIBRARY_RELEASE=$(THIS_DIR)extdeps/BZip2/lib/libbz2.dylib

ifeq ($(BUILD_TYPE), Debug)
	BZIP2_LIB_TYPE := -DBZIP2_LIBRARY_DEBUG=$(THIS_DIR)extdeps/BZip2/lib/libbz2.dylib
	BUILD_SYMBOL:=d
endif

ZLIB_LIB := -DZLIB_LIBRARY=$(THIS_DIR)extdeps/zlib/lib/libz.dylib \
			-DZLIB_INCLUDE_DIR=$(THIS_DIR)extdeps/zlib

PNG_LIB := 	-DPNG_LIBRARY=$(THIS_DIR)extdeps/libpng/lib/libpng16$(BUILD_SYMBOL).dylib \
			-DPNG_PNG_INCLUDE_DIR=$(THIS_DIR)extdeps/libpng/include

BZIP2_LIB := -DBZIP2_LIBRARIES=$(THIS_DIR)extdeps/BZip2/lib/libbz2.1.dylib 	\
			 -DBZIP2_INCLUDE_DIR=$(THIS_DIR)extdeps/BZip2/include 			\

BROTLI_LIB := -DBROTLIDEC_LIBRARIES=$(THIS_DIR)extdeps/brotli/lib/libbrotlidec.dylib \
			  -DBROTLIDEC_INCLUDE_DIRS=$(THIS_DIR)extdeps/brotli/include

HarfBuzz_LIB := -DHarfBuzz_LIBRARIES=$(THIS_DIR)extdeps/HarfBuzz/lib/libharfbuzz.a \
				-DHarfBuzz_INCLUDE_DIR=$(THIS_DIR)extdeps/HarfBuzz/include

FREEYTPE_LIB := -DFREETYPE_LIBRARY=$(THIS_DIR)extdeps/freetype/lib/libfreetype$(BUILD_SYMBOL).dylib \
				-DFREETYPE_INCLUDE_DIRS=$(THIS_DIR)extdeps/freetype/include

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

	sudo cmake -S downloads/BZip2 -B downloads/BZip2/build -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	sudo cmake --build downloads/BZip2/build --parallel
	sudo cmake --install downloads/BZip2/build --prefix extdeps/BZip2 --config $(BUILD_TYPE)

	sudo cmake -S downloads/libpng -B downloads/libpng/build \
	-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) $(ZLIB_LIB)

	sudo cmake --build downloads/libpng/build --parallel
	sudo cmake --install downloads/libpng/build --prefix extdeps/libpng --config $(BUILD_TYPE)

	sudo cmake -S downloads/HarfBuzz -B downloads/HarfBuzz/build -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	sudo cmake --build downloads/HarfBuzz/build --parallel
	sudo cmake --install downloads/HarfBuzz/build --prefix extdeps/HarfBuzz --config $(BUILD_TYPE)

	sudo cmake -S downloads/freetype -B downloads/freetype/build 						\
		$(ZLIB_LIB) $(PNG_LIB) $(BZIP2_LIB) $(BZIP2_LIB_TYPE) $(BROTLI_LIB) 			\
		$(HarfBuzz_LIB) -DCMAKE_INSTALL_PREFIX=extdeps/freetype 						\
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DCMAKE_PREFIX_PATH="$(THIS_DIR)extdeps/BZip2/"\
		-DBUILD_SHARED_LIBS=ON

	sudo cmake --build downloads/freetype/build --parallel
	sudo cmake --install downloads/freetype/build

	cmake -S downloads/msdf-atlas-gen -B downloads/msdf-atlas-gen/build 	\
		$(ZLIB_LIB) $(FREEYTPE_LIB) $(PNG_LIB) $(BZIP2_LIB) $(BROTLI_LIB)	\
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE)									\
		-DMSDF_ATLAS_USE_VCPKG=OFF -DMSDF_ATLAS_USE_SKIA=OFF

	cmake --build downloads/msdf-atlas-gen/build --parallel

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
	sudo rm -rf downloads/msdf-atlas-gen/build

configure:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) $(PNG_LIB)
build:
	cmake --build build --parallel

run:
	./build/glib

clear:
	rm -rf dbuild rbuild

clearAllCache:
	sudo rm -rf dbuild rbuild venv extdeps downloads