#include "template.glsl"

#if defined(__VERTEX_SHADER_TYPE__)

void main() {
    gl_Position = u_MVP * l_Position;
    transfer_variables_to_fs();
}

#elif defined(__FRAGMENT_SHADER_TYPE__)

uniform float u_Time;
uniform vec2 u_Resolution;

#define VAN_GOGH

#if defined(VAN_GOGH)
#include "van_gogh.glsl"
#elif defined(WAVE)
#include "wave.glsl"
#else
#include "anime_chan.glsl"
#endif

void main() {
    mainImage();
}

#endif
