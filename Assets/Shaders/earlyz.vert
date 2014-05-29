#version 400

layout(location = 0) in vec3 in_position;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

void main()
{
	gl_Position = projectionMatrix * viewMatrix * vec4( in_position, 1.0 );
}
