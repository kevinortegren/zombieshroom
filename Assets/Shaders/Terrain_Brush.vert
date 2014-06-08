#version 430

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
	vec4 g_pickPosAndRaius; //w contains radius of ring in world space
	vec3 g_cameraPos;
};

out vec4 world_pos;

void main()
{
	world_pos = modelMatrix * vec4( in_position, 1.0 );
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( in_position + vec3(0, 0.5, 0), 1.0 );
}
