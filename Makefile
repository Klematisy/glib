
configure:
	cmake -S . -B cmake-build-debug

install:
	cmake --build cmake-build-debug --target installer --parallel
build:
	cmake --build cmake-build-debug --target glib --parallel

run:
	./cmake-build-debug/glib