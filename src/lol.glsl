#include "../resources/shaders/template.glsl"

#if defined(SHADER_VERTEX)

void main() {
    transfer_varabiles_to_fs();
    gl_Position = u_MVP * l_Position;
}

#elif defined(SHADER_FRAGMENT)

void main() {
    o_Color = get_texture() * v_Color;
}

#endif
