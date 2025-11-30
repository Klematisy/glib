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


## Build
You can also specify your build type

### Windows:
```cmd
configure.bat install
configure.bat build_dependencies <YOUR BUILD TYPE>
configure.bat configure <YOUR BUILD TYPE>
```

### MacOS:
```bash
make install
make build_dependencies BUILD_TYPE=<YOUR BUILD TYPE>
make configure BUILD_TYPE=<YOUR BUILD TYPE> LINKING_TYPE=<YOUR LINKING TYPE>
cmake --build build --parallel
```