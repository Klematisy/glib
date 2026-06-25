#include "template.glsl"

#if defined(__VERTEX_SHADER_TYPE__)

void main() {
    gl_Position = u_MVP * l_Position;
    transfer_variables_to_fs();
}

#elif defined(__FRAGMENT_SHADER_TYPE__)

void main() {
    if (v_Color.a < 0.1) discard;
    o_Color = get_texture() * v_Color;
}

#endif
