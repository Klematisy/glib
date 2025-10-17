#include "template.glsl"

#define M_PI 3.1415926535

#if defined(SHADER_VERTEX)

void main() {
    basic_instructions();
    gl_Position = u_MVP * l_Position;
}

#elif defined(SHADER_FRAGMENT)

void main(void)
{
    o_Color = get_texture() * v_Color;
}

#endif