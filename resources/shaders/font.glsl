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

float screenPxRange() {
    float pxRange = 6.0;
    vec2 unitRange = vec2(pxRange) / vec2(textureSize(u_Texture, 0));
    vec2 screenTexSize = vec2(1.0) / fwidth(v_TexCoords.xy);
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

const vec4 fgColor = vec4(0.1, 0.3, 0.4, 1.0);
const vec4 outlineColor = vec4(0.95, 0.4, 0.3, 1.0);

// Range: -0.4 < midBodyThickness < 0.4
float midBodyThickness = -0.1;
// Range: 0.0 < outlineThickness < (0.4 - midBodyThickness)
float outlineThickness = 0.0;

void main() {
    vec4 texel = texture(u_Texture, v_TexCoords);
    float dist = median(texel.r, texel.g, texel.b);
    if (dist <= 0.0001) {
        discard;
    }
    float pxRange = screenPxRange();

    dist += -0.5 + midBodyThickness;

    float bodyPxDist = pxRange * dist;
    float bodyOpacity = smoothstep(-0.5, 0.5, bodyPxDist);

    float t = outlineThickness * abs(cos(2));

    float charPxDist = pxRange * (dist + t);
    float charOpacity = smoothstep(-0.5, 0.5, charPxDist);

    float outlineOpacity = charOpacity - bodyOpacity;

    vec3 color = mix(outlineColor.rgb, v_Color.rgb, bodyOpacity);
    float alpha = bodyOpacity * v_Color.a + outlineOpacity * outlineColor.a;

    o_Color = vec4(color, alpha);
}

#endif