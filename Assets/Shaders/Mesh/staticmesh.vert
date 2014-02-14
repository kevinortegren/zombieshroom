#version 400

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

out vec3 vert_normal;
out vec2 vert_texcoord;
out vec4 view;

void main()
{
	vert_normal = normalize( viewMatrix * vec4(in_normal, 0.0f)).xyz;
	vert_texcoord = in_texcoord;

	view = viewMatrix * vec4( in_position, 1.0f );

	gl_Position = projectionMatrix * viewMatrix * vec4( in_position, 1.0f );

}
