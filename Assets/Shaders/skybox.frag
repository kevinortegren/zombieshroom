#version 400

in vec3 vert_texcoord;
in vec3 vert_normal;

uniform samplerCube CubeMap;

layout (location = 2) out vec4 background;

void main()
{
	vec3 frag_color = texture(CubeMap, vert_texcoord).xyz;

	background = vec4(frag_color, 1.0f);
}