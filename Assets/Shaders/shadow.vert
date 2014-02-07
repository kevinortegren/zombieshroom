#version 400

layout(location = 0) in vec3 in_position;

layout(std140) uniform PerFrame
{
	mat4 viewProjection;
};

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
};

void main()
{
	gl_Position = viewProjection * modelMatrix * vec4(in_position, 1.0);
}
