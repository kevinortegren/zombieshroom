#version 400

in vec2 gs_TexCoord;

uniform sampler2D g_Diffuse;

layout (location = 2) out vec4 background;

void main(void) {

	vec4 frag_color = texture(g_Diffuse, gs_TexCoord);

	background = vec4(frag_color);
}