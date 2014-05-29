#version 430

uniform sampler2D g_Input;

in vec2 ex_TexCoord;

out vec4 out_Color;

void main() {

	out_Color = texture(g_Input, ex_TexCoord);
}