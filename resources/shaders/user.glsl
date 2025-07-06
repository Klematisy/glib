#if defined(SHADER_VERTEX)

void main() {
    basic_instructions();
    gl_Position = u_MVP * l_Position;
}

#elif defined(SHADER_FRAGMENT)

// o_Color - output data
// v_Color - input color
uniform float u_Time;

void main() {
    float wave = sin(v_TexCoords.x * 10.0 + u_Time);

//    vec4 time = vec4(u_Time + v_Color.g, u_Time + v_Color.r, u_Time + v_Color.b, v_Color.a);
    o_Color = basic_instructions() * vec4(vec3(0.5 + 0.5 * wave), 1.0);
}
#endif