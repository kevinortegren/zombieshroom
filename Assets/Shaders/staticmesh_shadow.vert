#version 430

layout(location = 0) in vec3 in_position;

layout(std140) uniform PerFrame
{
	mat4 viewProjection;
};

void main()
{
	gl_Position = viewProjection * vec4(in_position, 1.0);
}
