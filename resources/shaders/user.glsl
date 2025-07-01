#if defined(SHADER_VERTEX)

void main() {
    gl_Position = basic_instructions();
}

#elif defined(SHADER_FRAGMENT)

// o_Color - output data
// v_Color - input color
uniform float u_Time;

void main() {
    vec4 col = vec4(u_Time + v_Color.g, u_Time + v_Color.r, u_Time + v_Color.b, v_Color.a);
    o_Color = basic_instructions() * vec4(abs(sin(col.r)), abs(sin(col.g)) / 2, abs(cos(col.b)), 1.0);
}
#endif