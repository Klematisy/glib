
ROOT_DIR := $(dir $(firstword $(MAKEFILE_LIST)))

VENV_DIR := venv
VENV_PYTHON := $(VENV_DIR)/bin/python
VENV_PIP := $(VENV_DIR)/bin/pip

configure:
	cmake -S . -B cmake-build-debug

install:
	$(VENV_PYTHON) -m venv $(VENV_DIR)
	$(VENV_PYTHON) install_script.py
	$(VENV_PIP) install requests
build:
	cmake --build cmake-build-debug --target glib --parallel

run:
	./cmake-build-debug/glib