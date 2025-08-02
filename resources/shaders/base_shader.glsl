#if defined(SHADER_VERTEX)

layout(location = 0) in vec4 l_Position;
layout(location = 1) in vec4 l_Color;
layout(location = 2) in vec2 l_TexCoords;

out vec4  v_Color;
out vec2  v_TexCoords;

uniform mat4 u_MVP;

void basic_instructions() {
    gl_Position = u_MVP * l_Position;
    v_Color = l_Color;
    v_TexCoords = l_TexCoords;
}

#elif defined(SHADER_FRAGMENT)

layout(location = 0) out vec4 o_Color;

in vec4  v_Color;
in vec2  v_TexCoords;

uniform sampler2D u_Texture;

void basic_instructions() {
    o_Color = texture(u_Texture, v_TexCoords) * v_Color;
}

#endif


#if defined(SHADER_VERTEX)

void main() {
    basic_instructions();
}

#elif defined(SHADER_FRAGMENT)

void main() {
    basic_instructions();
}

#endif