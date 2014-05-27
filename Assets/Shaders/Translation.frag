#version 400

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 invView;
	mat4 invProjection;
};

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
	mat4 normalMatrix;
};

uniform sampler2D g_Color;

in vec2 fs_texcoord;

out vec4 frag_color;

void main()
{
	frag_color = vec4(texture(g_Color, fs_texcoord.st).rgb, 1.0);
}