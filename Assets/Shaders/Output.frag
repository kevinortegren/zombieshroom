#version 400

in vec2 ex_TexCoord;
out vec4 out_Color;

uniform sampler2D g_Texture;

void main() {
    out_Color = texture(g_Texture, ex_TexCoord);
}