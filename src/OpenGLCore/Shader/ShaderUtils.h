
#include <iostream>
#include <fstream>
#include "GL/glew.h"

namespace GlCore {

    class ShaderSourceLoader {
    public:
        static std::string Parse(const char* filePath);
    };

    class ShaderCreator {
    public:
        static uint32_t CreateShader(const std::string &m_ShaderSrc, uint32_t type);

        static void CheckShaderErrors(uint32_t shader, const char *specified_shader);

        static void CheckLinkingErrors(uint32_t shader);

        static int GetMaxSlotsCount();
    };

}