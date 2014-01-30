#version 410 core 

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
	mat4 normalMatrix;
	vec3 gEyeWorldPos;
};

out vec3 WorldPos_CS_in;
out vec2 TexCoord_CS_in;

void main()
{  
	TexCoord_CS_in	= in_texcoord;
	WorldPos_CS_in	= (modelMatrix * vec4( in_position, 1.0f )).xyz;
}
