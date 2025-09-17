
ROOT_DIR := $(dir $(firstword $(MAKEFILE_LIST)))

VENV_DIR := venv
VENV_PYTHON := $(VENV_DIR)/bin/python
VENV_PIP := $(VENV_DIR)/bin/pip

_configure_:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

_install_:
	python3 -m venv venv
	$(VENV_PIP) install requests
	$(VENV_PYTHON) install_script.py
_build_:
	cmake --build build --target glib --parallel

_run_:
	./build/glib