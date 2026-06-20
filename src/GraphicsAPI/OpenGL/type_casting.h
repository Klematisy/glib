#pragma once

#include "../graphics_api.h"

#include "GL/glew.h"
#define TO_INT(X) static_cast<int>(X)

struct GL {
    static constexpr int BUFFER_TYPE[] = { GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_FRAMEBUFFER, GL_RENDERBUFFER };
    static constexpr int DRAW_TYPE[] = { GL_STATIC_DRAW, GL_DYNAMIC_DRAW };

    static constexpr int RENDERER_TYPE[] = { GL_TRIANGLES, GL_LINES, GL_POINTS };
    static constexpr int CLEAR_BUFFER_BIT[] = { GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT };

    static constexpr int SHADER_TYPE[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER, GL_GEOMETRY_SHADER };
    static constexpr int SHADER_COMPILE[] = { GL_COMPILE_STATUS, GL_INFO_LOG_LENGTH };
    static constexpr int SHADER_PROGRAM_COMPILE[] = { GL_LINK_STATUS, GL_INFO_LOG_LENGTH };

    static constexpr int TEXTURE_TYPE[] = { GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_2D_ARRAY };
    static constexpr int FORMAT[] = { GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_RGBA };
    static constexpr int INTERNAL_FORMAT[] = { GL_RGB8, GL_RGBA8, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, GL_COLOR_ATTACHMENT0 };
    static constexpr int TEXTURE_PROPERTY[] = { GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T };
    static constexpr int TEXTURE_PARAM[] = { GL_NEAREST, GL_LINEAR, GL_CLAMP_TO_EDGE };

    static constexpr int BLEND_PARAM[] = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };

    static GLenum ConvertAPITypeToGlType(GAPI::API_TYPE type);
};
