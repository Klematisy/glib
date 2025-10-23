#pragma once

#if defined(SHADER_VERTEX)

layout(location = 0) in vec4 l_Position;
layout(location = 1) in vec4 l_Color;
layout(location = 2) in vec3 l_TexCoords;

out vec4 v_Color;
out vec3 v_TexCoords;


uniform mat4 u_MVP;

void transfer_varabiles_to_fs() {
    v_Color = l_Color;
    v_TexCoords = l_TexCoords;
}

#elif defined(SHADER_FRAGMENT)

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec3 v_TexCoords;

uniform sampler2DArray u_Texture;

vec4 get_texture() {
    return texture(u_Texture, v_TexCoords);
}

#endif