#include "template.glsl"

#if defined(__VERTEX_SHADER_TYPE__)

void main() {
    gl_Position = u_MVP * l_Position;
    transfer_varabiles_to_fs();
}

#elif defined(__FRAGMENT_SHADER_TYPE__)

uniform float u_Time;
uniform vec2 u_Resolution;

#define PI 3.14159265

vec3 palette(float t) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.1, 0.4, 0.5);
    return a + b * cos(2.0 * PI * (c * t + d));
}

vec4 wave(vec2 uv, vec4 color, float amp, float freq, float phase, float thick, vec3 hue) {
    float x = uv.x - phase;
    float y = uv.y + amp * sin(freq * x);
    float bright = smoothstep(0.0, 1.0, 1.0 - abs(y) / thick);
    return vec4(vec3(bright) * hue, 1.0);
}

void mainImage() {
    vec2 uv = (1.0 * gl_FragCoord.xy - u_Resolution.xy) / u_Resolution.y;

    o_Color = vec4(0.0, 0.0, 0.0, 1.0);

    for (float layer = 0.0; layer < 1.0; layer += 0.1) {
        float amp = 0.25 + 0.25 * sin(u_Time + layer) * (1.0 - layer);
        float freq = 2.0;
        float phase = u_Time * (1.0 - layer);
        float thick = 0.01 + 0.001 * pow(abs(uv.x), 8.0);
        vec3 hue = palette(0.5 * uv.x + 1.0 * layer - 0.5 * u_Time);
        o_Color += wave(uv, o_Color, amp, freq, phase, thick, hue);
    }
}

void main() {
    mainImage();
}

#endif
