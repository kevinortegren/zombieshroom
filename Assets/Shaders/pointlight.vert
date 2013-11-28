#version 400

layout(location = 0) in vec2 in_PositionM;
layout(location = 1) in vec2 in_TexCoord;

in int gl_InstanceID;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 invProj;
	mat4 invViewProj;
};

struct DirectionalLight
{
	vec3 LightDirection;
	vec4 Color;
};

struct PointLight
{
	vec3 LightPosition;
	vec3 Attenuation;	
	float Range;
	vec4 Color;
};

// Uniform data.
layout(std140) uniform Lights
{
	vec4 g_Ambient;
	DirectionalLight dlights[16];
	PointLight plights[16];
};

out vec2 ex_TexCoord;
out PointLight ex_Light;

void main() {
    gl_Position = vec4(vec3(in_PositionM, 0.0f), 1.0f);
    ex_TexCoord = in_TexCoord;

	ex_Light = plights[gl_InstanceID];
}