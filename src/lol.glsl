#include "../resources/shaders/template.glsl"

#if defined(SHADER_VERTEX)

void main() {
    transfer_varabiles_to_fs();
    gl_Position = u_MVP * l_Position;
}

#elif defined(SHADER_FRAGMENT)

void main() {
    vec4 col = vec4(1.0, 0.6, 0.2, 1.0);
    o_Color = get_texture() * col;
}

#endif
