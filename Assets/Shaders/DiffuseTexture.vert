#version 400

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texCoord;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
	mat4 normalMatrix;
};

layout(std140) uniform Lights
{
	vec3 lightDirection;
};

out Fragment
{
	out vec2 texCoord;
} fragment;

void main()
{
	fragment.texCoord = in_texCoord;

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( in_position, 1.0f );
}
