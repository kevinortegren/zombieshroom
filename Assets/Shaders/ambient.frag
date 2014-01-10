#version 400

in vec2 ex_TexCoord;
out vec4 out_Color;

layout(std140) uniform Lights
{
	vec4 g_Ambient;
};

void main() {
    out_Color = vec4(g_Ambient.xyz, 1.0f);
}