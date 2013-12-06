#version 400

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
};

out vec4 vert_color;

void main()
{
	vert_color = in_color;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( in_position, 1.0f );
}
