#version 400

in vec2 ex_TexCoord;
out vec4 out_Color;

uniform sampler2D g_Diffuse;
uniform sampler2D g_Normals;

layout(std140) uniform Ambient
{
	vec4 g_Ambient;
};

void main() {
    out_Color = texture(g_Diffuse, ex_TexCoord) * g_Ambient;
}