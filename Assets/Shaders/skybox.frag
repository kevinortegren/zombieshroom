#version 400

in vec3 vert_texcoord;
in vec3 vert_normal;

uniform samplerCube CubeMap;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec3 normals;

void main()
{
	vec3 frag_color = texture(CubeMap, vert_texcoord).xyz;
	vec3 normal = normalize(vert_normal);

	diffuse = vec4(frag_color, 1.0f);
	normals = vec3(0.0f);
}