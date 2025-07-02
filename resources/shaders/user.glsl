uniform float u_Time;

#if defined(SHADER_VERTEX)

void main() {
    basic_instructions();

    gl_Position = u_MVP * l_Position;
}

#elif defined(SHADER_FRAGMENT)

// o_Color - output data
// v_Color - input color

void main() {
    vec4 col = vec4(u_Time + v_Color.g, u_Time + v_Color.r, u_Time + v_Color.b, v_Color.a);
    col = vec4(abs(sin(col.r)), abs(sin(col.g)) / 2, abs(cos(col.b)), 1.0);
    o_Color = basic_instructions() * col;
}
#endif