
# BUILD_TYPE - variable for specifying the build type {Release, Debug}
# Basic build type
BUILD_TYPE ?= Release
BUILD_DEPS ?= ON
BUILD_FOLDER_NAME=r

VENV_DIR := venv
VENV_PYTHON := $(VENV_DIR)/bin/python
VENV_PIP := $(VENV_DIR)/bin/pip

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

configure:
ifeq ($(BUILD_DEPS), ON)
	cmake -S $(FREETYPE_DIR) -B $(FREETYPE_DIR)/$(BUILD_FOLDER_NAME)build -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
endif
	cmake -S . -B $(BUILD_FOLDER_NAME)build -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

build:
ifeq ($(BUILD_DEPS), ON)
	cmake --build $(FREETYPE_DIR)/$(BUILD_FOLDER_NAME)build --parallel
endif
	cmake --build $(BUILD_FOLDER_NAME)build --parallel

run:
	./build/glib