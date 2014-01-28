#version 400

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

uniform sampler2D g_Diffuse;

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

out vec2 vert_texcoord;
out vec4 view;

void main()
{
	vert_texcoord = in_texcoord;

	view = viewMatrix * modelMatrix * vec4( in_position.x, in_position.y + height, in_position.z , 1.0f );
	float height = texture(g_Diffuse, in_texcoord).r;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( in_position.x, in_position.y + height, in_position.z , 1.0f );
}
