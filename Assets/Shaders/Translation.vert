#version 400

layout(location = 0) in vec3 in_position;
layout(location = 2) in vec2 in_texcoord;

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

out vec2 fs_texcoord;

void main()
{
	fs_texcoord = in_texcoord;

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_position, 1.0);
}
