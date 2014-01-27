#version 400

uniform sampler2D g_Input;

in vec2 ex_TexCoord;

out vec4 out_Color;

void main() {

	out_Color = texture(g_Background, ex_TexCoord);
}