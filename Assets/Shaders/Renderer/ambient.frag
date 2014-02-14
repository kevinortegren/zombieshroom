#version 400

in vec2 ex_TexCoord;
out vec4 out_Color;

uniform sampler2D g_Diffuse;

layout(std140) uniform Lights
{
	vec4 g_Ambient;
};

void main() {
    out_Color = vec4(g_Ambient.xyz * texture(g_Diffuse, ex_TexCoord.st).xyz, 1.0f);
}