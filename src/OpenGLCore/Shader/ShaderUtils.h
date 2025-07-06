
#include <iostream>
#include <fstream>
#include "GL/glew.h"

#include "../Logger.h"

namespace GlCore {

    class ShaderSourceLoader {
    public:
        static std::string Parse(const char* filePath);
    };

    class ShaderCreator {
    public:
        static int CreateShader(const std::string &m_ShaderSrc, uint32_t type);
        static int CheckShaderErrors(uint32_t shader, const char *specified_shader);
        static int CheckLinkingErrors(uint32_t shader);

        static int GetMaxSlotsCount();
    };

}