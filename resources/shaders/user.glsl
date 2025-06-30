#if defined(SHADER_VERTEX)

void main() {
    gl_Position = basic_instructions();
}

#elif defined(SHADER_FRAGMENT)

// o_Color - output data
// v_Color - input color
uniform float u_Time;

void main() {
    vec4 time = vec4(u_Time + v_Color.g, u_Time + v_Color.r, u_Time + v_Color.b, v_Color.a);
    o_Color = basic_instructions() * vec4(abs(sin(time.r)), abs(cos(time.g)), abs(sin(time.b)), 1.0);
}
#endif