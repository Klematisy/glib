
ROOT_DIR := $(dir $(firstword $(MAKEFILE_LIST)))

VENV_DIR := venv
VENV_PYTHON := $(VENV_DIR)/bin/python
VENV_PIP := $(VENV_DIR)/bin/pip

MSDFGEN_DIR := extdeps/msdfgen
FREETYPE_DIR := extdeps/freetype

build_dependencies:
	cmake -S $(FREETYPE_DIR) -B $(FREETYPE_DIR)/build -DCMAKE_PREFIX_PATH="extdeps/freetype_install"
	cmake --build $(FREETYPE_DIR)/build --parallel
	cmake --install $(FREETYPE_DIR)/build

	cmake -S $(MSDFGEN_DIR) -B $(MSDFGEN_DIR)/build -DCMAKE_PREFIX_PATH="extdeps/freetype_install/lib/cmake/freetype"
	cmake --build $(MSDFGEN_DIR)/build --parallel
install:
	python3 -m venv venv
	$(VENV_PIP) install requests
	$(VENV_PYTHON) install_script.py

configure:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

build_glib:
	cmake --build build --target glib --parallel

run:
	./build/glib