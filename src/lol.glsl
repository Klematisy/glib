#pragma once



#if defined(SHADER_FRAGMENT)

vec4 lol(vec4 col) {
    col *= 8;
    return col;
}

#endif
