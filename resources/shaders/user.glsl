#include "template.glsl"

#define M_PI 3.1415926535

#if defined(SHADER_VERTEX)

// u_MVP - all input matrices

//uniform mat4 u_Proj;

void main() {
    basic_instructions();
    gl_Position = u_MVP * l_Position;
}

#elif defined(SHADER_FRAGMENT)

// o_Color - output data
// v_Color - input color
// in vec3 v_TexCoords;

void main(void)
{
    o_Color = get_texture() * v_Color;
}

#endif