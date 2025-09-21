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
uniform float u_Time;

float ctr(float angle) {
    return angle * M_PI / 180.0;
}

void main() {
    int delta = 2;

    vec4 time = vec4(abs(sin(u_Time)),
                     abs(cos(u_Time)),
                     abs(sin(u_Time)), 1.0);
    o_Color = basic_instructions() * (v_Color * time);
}
#endif