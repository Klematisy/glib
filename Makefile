
# BUILD_TYPE - variable for specifying the build type {Release, Debug}
# Basic build type
BUILD_TYPE ?= Release
LINKING_TYPE ?= STATIC

THIS_MAKEFILE=$(lastword $(MAKEFILE_LIST))
THIS_DIR=$(dir $(realpath $(THIS_MAKEFILE)))

VENV_DIR=.venv
VENV_PYTHON=$(VENV_DIR)/bin/python
VENV_PIP=$(VENV_DIR)/bin/pip
ARCH := $(shell uname -m)

BZIP2_LIB_TYPE=-DBZIP2_LIBRARY_RELEASE=$(THIS_DIR)extdeps/BZip2/lib/libbz2_static.a

ifeq ($(BUILD_TYPE), Debug)
	BZIP2_LIB_TYPE=-DBZIP2_LIBRARY_DEBUG=$(THIS_DIR)extdeps/BZip2/lib/libbz2_static.a
	BUILD_SYMBOL=d
endif

FREETYPE_LIB=-DFREETYPE_LIBRARY=$(THIS_DIR)extdeps/freetype/lib/libfreetype$(BUILD_SYMBOL).a \
			 -DFREETYPE_INCLUDE_DIRS=$(THIS_DIR)extdeps/freetype/include

MSDFATLASGEN_LIB=-DMSDF_ATLAS_GEN_LIBRARY="$(THIS_DIR)extdeps/msdf-atlas-gen/build/libmsdf-atlas-gen.a"	\
				 -DMSDFGEN_LIBRARIES="$(THIS_DIR)extdeps/msdf-atlas-gen/build/msdfgen/libmsdfgen-core.a;$(THIS_DIR)extdeps/msdf-atlas-gen/build/msdfgen/libmsdfgen-ext.a"

install:
	python3.13 -m venv $(VENV_DIR)
	$(VENV_PIP) install requests
	$(VENV_PYTHON) install_script.py

build_dependencies:
	sudo rm -rf "extdeps"
	sudo mkdir "extdeps"

	sudo cmake -S downloads/freetype -B downloads/freetype/build 				      \
		-DCMAKE_OSX_ARCHITECTURES=$(ARCH) -DCMAKE_INSTALL_PREFIX=extdeps/freetype     \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DFT_DISABLE_BZIP2=ON -DFT_DISABLE_BROTLI=ON \
		-DFT_DISABLE_HVF=ON -DFT_DISABLE_ZLIB=ON -DFT_DISABLE_BZIP2=ON                \
		-DFT_DISABLE_PNG=ON -DFT_DISABLE_HARFBUZZ=ON -DFT_DISABLE_BROTLI=ON
	sudo cmake --build downloads/freetype/build --parallel
	sudo cmake --install downloads/freetype/build


	cmake -S downloads/msdf-atlas-gen -B downloads/msdf-atlas-gen/build \
		-DCMAKE_OSX_ARCHITECTURES=$(ARCH) $(FREETYPE_LIB)               \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) 								\
		-DMSDF_ATLAS_USE_VCPKG=OFF -DMSDF_ATLAS_USE_SKIA=OFF			\
		-DMSDF_ATLAS_DYNAMIC_RUNTIME=ON -DBUILD_SHARED_LIBS=OFF			\
		-DMSDF_ATLAS_BUILD_STANDALONE=OFF
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
	cmake -S . -B build	                    \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON	\
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE)	\
		-DCMAKE_OSX_ARCHITECTURES=$(ARCH) 	\
		$(FREETYPE_LIB) $(MSDFATLASGEN_LIB) \
		-DLINKING_TYPE=$(LINKING_TYPE)
build:
	cmake --build build --parallel

run:
	./build/VLADLIB_TEST

clear:
	rm -rf dbuild rbuild build

clear_all:
	sudo rm -rf dbuild rbuild build .venv extdeps downloads
