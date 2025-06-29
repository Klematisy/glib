#if defined(SHADER_VERTEX)

void main() {
    gl_Position = basic_instructions();
}

#elif defined(SHADER_FRAGMENT)

// o_Color - output data
void main() {
    o_Color = basic_instructions();
    o_Color += 0.0;
}
#endif