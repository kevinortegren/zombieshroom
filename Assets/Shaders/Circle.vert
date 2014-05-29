#version 430

layout(location = 0) in vec3 in_position;

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
	vec4 color;
	vec2 viewport;
};

out float viewZ;

void main()
{
	viewZ = (viewMatrix * modelMatrix * vec4( in_position, 1.0 )).z;


	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_position, 1.0);
}
