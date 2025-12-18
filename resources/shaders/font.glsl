#include "template.glsl"

#if defined(SHADER_VERTEX)

void main() {
    transfer_varabiles_to_fs();
    gl_Position = u_MVP * l_Position;
}

#elif defined(SHADER_FRAGMENT)

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
    vec3 s = texture(u_Texture, v_TexCoords).rgb;
    float sigDist = median(s.r, s.g, s.b) - 0.5;

    float pxDist = sigDist * 4.0 / fwidth(sigDist);
    float opacity = clamp(pxDist + 0.5, 0.0, 1.0);


//    if (opacity < 0.01 || v_Color.a < 0.01)
//        discard;

    o_Color = vec4(v_Color.rgb, v_Color.a * opacity);
}


#endif