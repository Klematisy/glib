layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec3 v_TexCoords;

uniform sampler2DArray u_Texture;

void main() {
    if (v_Color.a < 0.1) discard;
    o_Color = texture(u_Texture, v_TexCoords) * v_Color;
}
