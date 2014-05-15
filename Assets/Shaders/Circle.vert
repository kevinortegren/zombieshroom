#version 400

layout(location = 0) in vec3 in_position;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
	mat4 normalMatrix;
	vec4 color;
};

out float viewZ;

void main()
{
	viewZ = (viewMatrix * modelMatrix * vec4( in_position, 1.0f )).z;


	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_position, 1.0);
}
