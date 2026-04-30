#include "../../resources/shaders/template.glsl"

#if defined(SHADER_VERTEX)

void main() {
    transfer_varabiles_to_fs();
    gl_Position = u_MVP * l_Position;
}

#elif defined(SHADER_FRAGMENT)

uniform float u_Time;

#define PI 3.14159

void main() {
    vec4 col = vec4(abs(sin(u_Time * PI / 180.0f) / 2),
                    abs(cos(u_Time * PI / 180.0f) / 0.99),
                    abs(cos((u_Time + 80) * PI / 180.0f) / 0.99),
                    1);

    o_Color = get_texture() * col;
}

#endif