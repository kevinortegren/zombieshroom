#version 400

in vec3 vert_texcoord;
in vec3 vert_normal;

uniform samplerCube CubeMap;

out vec4 out_Color;

void main()
{
	vec3 frag_color = texture(CubeMap, vert_texcoord).xyz;

	out_Color = vec4(frag_color, 1.0f);
}