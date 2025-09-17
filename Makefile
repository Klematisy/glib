
ROOT_DIR := $(dir $(firstword $(MAKEFILE_LIST)))

VENV_DIR := venv
VENV_PYTHON := $(VENV_DIR)/bin/python
VENV_PIP := $(VENV_DIR)/bin/pip

configure_glib:
	cmake -S . -B build

install_deps:
	python3 -m venv venv
	$(VENV_PIP) install requests
	$(VENV_PYTHON) install_script.py
build_glib:
	cmake --build build --target glib --parallel

run_glib:
	./build/glib