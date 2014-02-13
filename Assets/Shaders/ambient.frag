#version 400

in vec2 ex_TexCoord;
out vec4 out_Color;

uniform sampler2D g_Diffuse;
uniform sampler2D g_SSAO;

layout(std140) uniform Lights
{
	vec4 g_Ambient;
};

void main() {
	
    out_Color = vec4(g_Ambient.rgb * texture(g_SSAO, ex_TexCoord.st).r * texture(g_Diffuse, ex_TexCoord.st).rgb, 1.0f);
}