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
    float tileFactor;
};

out vec3 vert_normal;
out vec2 vert_texcoord;
out vec2 vert_texcoord_scaled;

void main()
{
	vert_normal = normalize( viewMatrix * vec4(in_normal, 0.0f)).xyz;
	vert_texcoord = in_texcoord;
    vert_texcoord_scaled = in_texcoord * tileFactor;

	gl_Position = projectionMatrix * viewMatrix * vec4( in_position, 1.0f );

}
