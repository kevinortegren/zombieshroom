#version 410 core 

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;
layout(location = 3) in vec3 in_tangent;
layout(location = 4) in vec3 in_bitangent;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 invView;
	mat4 invProj;
	mat4 invViewProj;
};  

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
	mat4 normalMatrix;
	vec3 gEyeWorldPos;
	float gTime;
	vec4 gOptions;
	vec4 SunColor;
	vec3 SunDirection;
	float dx2; //Length between height values
}; 

out vec3 WorldPos_CS_in;
out vec2 TexCoord_CS_in;

void main()
{  
	TexCoord_CS_in	= in_texcoord;
	WorldPos_CS_in	= (modelMatrix * vec4( in_position, 1.0 )).xyz;
}
