# GLIB

A modular, cross-platform C++ rendering library built on OpenGL with support for custom shaders, texture atlases, mesh and text rendering, offscreen framebuffer baking, and an extensible architecture designed for future migration to Vulkan.

---

## Features
- ✅ **Mesh rendering**
- ✅ **Text rendering**
- ✅ **Custom GLSL shader system with the include directive**
- ✅ **Custom camera system**
- ✅ **Framebuffer rendering**
- ✅ **Modular architecture**
- ✅ **Cross-platform build system (Windows / macOS, Linux - in dev)**
- ✅ **Support migration to Vulkan**
---

## Build & Dependencies
You can download the dependencies for compile glib. If you wanna do this you need to have already installed python and virtualenv
### Windows:
Build on windows with dependencies downloaded:
```cmd
configure.bat install
configure.bat build_dependencies <Debug/Release>
configure.bat configure <Debug/Release> <STATIC/DYNAMIC>
cmake --build build --parallel --config <Debug/Release>
```
If you don't want to download the dependencies - you can already use:
```cmd
cmake -S . -B build -DCMAKE_BUILD_TYPE=<Debug/Release> LINKING_TYPE=<STATIC/DYNAMIC>
cmake --build build --parallel --config <Debug/Release>
```


### MacOS:
Build on MacOS with dependencies downloaded:
```bash
make install
make build_dependencies BUILD_TYPE=<Debug/Release>
make configure BUILD_TYPE=<Debug/Release> LINKING_TYPE=<STATIC/DYNAMIC>
cmake --build build --parallel
```
Build on windows without dependencies downloaded:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=<Debug/Release> LINKING_TYPE=<STATIC/DYNAMIC>
cmake --build build --parallel
```