
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

ZLIB_LIB=-DZLIB_LIBRARY=$(THIS_DIR)extdeps/zlib/lib/libz.a	\
		 -DZLIB_INCLUDE_DIR=$(THIS_DIR)extdeps/zlib

BZIP2_LIB=-DBZIP2_LIBRARY=$(THIS_DIR)extdeps/BZip2/lib/libbz2_static.a \
		  -DBZIP2_INCLUDE_DIR=$(THIS_DIR)extdeps/BZip2/include 			\

BROTLIDEC_LIB=-DBROTLIDEC_LIBRARIES="$(THIS_DIR)extdeps/brotli/lib/libbrotlidec-static.a" \
		  	  -DBROTLIDEC_INCLUDE_DIRS=$(THIS_DIR)extdeps/brotli/include

BROTLICOMMON_LIB=-DBROTLICOMMON_LIBRARY="$(THIS_DIR)extdeps/brotli/lib/libbrotlicommon-static.a" \
				 -DBROTLICOMMON_INCLUDE_DIRS=$(THIS_DIR)extdeps/brotli/include

PNG_LIB=-DPNG_LIBRARY=$(THIS_DIR)extdeps/libpng/lib/libpng16$(BUILD_SYMBOL).a	\
		-DPNG_PNG_INCLUDE_DIR=$(THIS_DIR)extdeps/libpng/include

HarfBuzz_LIB=-DHarfBuzz_LIBRARIES=$(THIS_DIR)extdeps/HarfBuzz/lib/libharfbuzz.a \
			 -DHarfBuzz_INCLUDE_DIR=$(THIS_DIR)extdeps/HarfBuzz/include

FREETYPE_LIB=-DFREETYPE_LIBRARY=$(THIS_DIR)extdeps/freetype/lib/libfreetype$(BUILD_SYMBOL).a \
			 -DFREETYPE_INCLUDE_DIRS=$(THIS_DIR)extdeps/freetype/include

MSDFATLASGEN_LIB=-DMSDF_ATLAS_GEN_LIBRARY="$(THIS_DIR)extdeps/msdf-atlas-gen/build/libmsdf-atlas-gen.a"	\
				 -DMSDFGEN_LIBRARIES="$(THIS_DIR)extdeps/msdf-atlas-gen/build/msdfgen/libmsdfgen-core.a;$(THIS_DIR)extdeps/msdf-atlas-gen/build/msdfgen/libmsdfgen-ext.a"

install:
	virtualenv .venv
	$(VENV_PIP) install requests
	$(VENV_PYTHON) install_script.py

build_dependencies:
	sudo rm -rf "extdeps"
	sudo mkdir "extdeps"

	sudo cmake -S downloads/brotli -B downloads/brotli/build \
		-DCMAKE_OSX_ARCHITECTURES=$(ARCH)  					 \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) 					 \
		-DBROTLI_BUILD_FOR_PACKAGE=ON
	sudo cmake --build downloads/brotli/build --parallel
	sudo cmake --install downloads/brotli/build --prefix extdeps/brotli --config $(BUILD_TYPE)

	sudo cmake -S downloads/BZip2 -B downloads/BZip2/build 	\
		-DCMAKE_OSX_ARCHITECTURES=$(ARCH) 					\
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE)					\
		-DENABLE_SHARED_LIB=OFF 							\
		-DENABLE_STATIC_LIB=ON 								\
		-DENABLE_STATIC_LIB_IS_PIC=ON
	sudo cmake --build downloads/BZip2/build --parallel
	sudo cmake --install downloads/BZip2/build --prefix extdeps/BZip2 --config $(BUILD_TYPE)


	sudo cmake -S downloads/zlib -B downloads/zlib/build 	\
		-DCMAKE_OSX_ARCHITECTURES=$(ARCH) 					\
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE)					\
		-DCMAKE_INSTALL_PREFIX="extdeps/zlib"
	sudo cmake --build downloads/zlib/build --parallel
	sudo cmake --install downloads/zlib/build --config $(BUILD_TYPE)


	sudo cmake -S downloads/libpng -B downloads/libpng/build 	\
		-DCMAKE_OSX_ARCHITECTURES=$(ARCH) 						\
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) $(ZLIB_LIB)			\
		-DPNG_SHARED=OFF -DPNG_STATIC=ON
	sudo cmake --build downloads/libpng/build --parallel
	sudo cmake --install downloads/libpng/build --prefix extdeps/libpng --config $(BUILD_TYPE)


	sudo cmake -S downloads/HarfBuzz -B downloads/HarfBuzz/build 	\
		-DCMAKE_OSX_ARCHITECTURES=$(ARCH) 							\
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE)							\
		-DBUILD_SHARED_LIBS=OFF
	sudo cmake --build downloads/HarfBuzz/build --parallel
	sudo cmake --install downloads/HarfBuzz/build --prefix extdeps/HarfBuzz --config $(BUILD_TYPE)


	sudo cmake -S downloads/freetype -B downloads/freetype/build 							\
		-DCMAKE_OSX_ARCHITECTURES=$(ARCH) 													\
		$(ZLIB_LIB) $(PNG_LIB) $(BZIP2_LIB) $(BZIP2_LIB_TYPE) $(BROTLIDEC_LIB) 				\
		$(HarfBuzz_LIB) -DCMAKE_INSTALL_PREFIX=extdeps/freetype $(BROTLICOMMON_LIB)			\
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DCMAKE_PREFIX_PATH="$(THIS_DIR)extdeps/BZip2/"	\
		-DFT_REQUIRE_BZIP2=ON -DFT_REQUIRE_BROTLI=ON
	sudo cmake --build downloads/freetype/build --parallel
	sudo cmake --install downloads/freetype/build


	cmake -S downloads/msdf-atlas-gen -B downloads/msdf-atlas-gen/build \
		-DCMAKE_OSX_ARCHITECTURES=$(ARCH) 								\
		$(ZLIB_LIB) $(PNG_LIB) $(FREETYPE_LIB) $(BZIP2_LIB)			 	\
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
	cmake -S . -B build	                        \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON		\
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE)		\
		-DCMAKE_OSX_ARCHITECTURES=$(ARCH) 		\
		$(PNG_LIB) $(FREETYPE_LIB) 				\
		$(BROTLI_LIB) $(BZIP2_LIB)				\
		$(ZLIB_LIB) $(BROTLIDEC_LIB)		   	\
		$(BROTLICOMMON_LIB) $(MSDFATLASGEN_LIB) \
		-DLINKING_TYPE=$(LINKING_TYPE)
build:
	cmake --build build --parallel

run:
	./build/VLADLIB_TEST

clear:
	rm -rf dbuild rbuild build

clear_all:
	sudo rm -rf dbuild rbuild build .venv extdeps downloads
