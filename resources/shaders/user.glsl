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

float ctr(float angle) {
    return angle * M_PI / 180.0;
}

void main() {
    float dist = texture(u_Texture, v_TexCoords).r;
    float width = fwidth(dist);

    float alpha = smoothstep(0.5 - width, 0.5 + width, dist);
    o_Color = v_Color;
    o_Color.a = alpha;
}
#endif