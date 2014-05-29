#version 430

layout(location = 0) in vec3 in_PositionM;
layout(location = 1) in vec2 in_TexCoord;

in int gl_InstanceID;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 invView;
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
    mat4 Transform;
};

// Uniform data.
layout(std140) uniform Lights
{
	vec4 g_Ambient;
	DirectionalLight dlights[5];
	PointLight plights[500];
};

out vec2 ex_TexCoord;
out PointLight ex_Light;

void main() {

	ex_Light = plights[gl_InstanceID];
	ex_Light.LightPosition = (viewMatrix * vec4(ex_Light.LightPosition, 1.0f)).xyz; 
    ex_TexCoord = in_TexCoord;
    
    gl_Position = projectionMatrix * viewMatrix * ex_Light.Transform * vec4(in_PositionM, 1.0f);
}