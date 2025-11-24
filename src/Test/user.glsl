#pragma once

#include "../resources/shaders/template.glsl"

#define M_PI 3.1415926535

#if defined(SHADER_VERTEX)

void main()
{
    transfer_varabiles_to_fs();
    gl_Position = u_MVP * l_Position;
}

#elif defined(SHADER_FRAGMENT)

void main(void)
{
    o_Color = get_texture() * v_Color;
}

#endif