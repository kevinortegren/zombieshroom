#version 430

layout(location = 0) in vec3 in_position;

layout(std140) uniform PerFrame
{
	mat4 viewProjectionMatrix;
};

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
};

void main()
{
	gl_Position = viewProjectionMatrix * modelMatrix * vec4(in_position, 1.0);
}
