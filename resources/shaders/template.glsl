#if defined(SHADER_VERTEX)

layout(location = 0) in vec4 l_Position;
layout(location = 1) in vec4 l_Color;
layout(location = 2) in vec2 l_TexCoords;
layout(location = 3) in float l_TexIndex;

out vec4  v_Color;
out vec2  v_TexCoords;
out float v_TexIndex;

uniform mat4 u_MVP;

vec4 basic_instructions() {
    v_Color = l_Color;
    v_TexCoords = l_TexCoords;
    v_TexIndex = l_TexIndex;
    return u_MVP * l_Position;
}

#elif defined(SHADER_FRAGMENT)

layout(location = 0) out vec4 o_Color;

in vec4  v_Color;
in vec2  v_TexCoords;
in float v_TexIndex;

uniform sampler2D u_Texture[MAX_SLOTS_COUNT];

vec4 basic_instructions() {
    o_Color = texture(u_Texture[int(v_TexIndex)], v_TexCoords) * v_Color;
    return o_Color;
}

#endif