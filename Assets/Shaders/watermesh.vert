#version 400

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
};
//uniform sampler2D g_Diffuse;

out vec2 vert_texcoord;

void main()
{
	vert_texcoord = in_texcoord;
	//float height = texture(g_Diffuse, in_texcoord).r;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( in_position.x, in_position.y, in_position.z, 1.0f );
}
