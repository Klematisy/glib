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
    vec3 d = vec3(0.263, 0.416, 0.557);

    return a + b * cos(6.28318 * (c * t + d));
}

void mainImage() {
    vec2 uv = (gl_FragCoord.xy - u_Resolution.xy) / u_Resolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);

    for (float i = 0.0; i < 4.0; i++) {
        uv = fract(uv * 1.5) - 0.5;

        float d = length(uv) * exp(-length(uv0));

        vec3 col = palette(length(uv0) + i * .4 + u_Time * .4);

        d = sin(d * 8. + u_Time) / 8.;
        d = abs(d);

        d = pow(0.01 / d, 1.2);

        finalColor += col * d;
    }

    o_Color = vec4(finalColor, 1.0);
}

void main() {
    mainImage();
}

#endif
