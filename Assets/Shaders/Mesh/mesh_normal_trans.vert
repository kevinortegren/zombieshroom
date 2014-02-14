#version 400

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;
layout(location = 3) in vec3 in_tangent;
layout(location = 4) in vec3 in_bitangent;

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

out vec3 vert_normal;
out vec2 vert_texcoord;
out vec3 vert_tangent;
out vec3 vert_bitangent;

void main()
{
	vert_normal = normalize( viewMatrix * normalMatrix * vec4(in_normal, 0.0f)).xyz;
	vert_tangent = normalize( viewMatrix * normalMatrix * vec4(in_tangent, 0.0f)).xyz;
	vert_bitangent = normalize( viewMatrix * normalMatrix * vec4(in_bitangent, 0.0f)).xyz;

	vert_texcoord = in_texcoord;

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( in_position, 1.0f );

}
