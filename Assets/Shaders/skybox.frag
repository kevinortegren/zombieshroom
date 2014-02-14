#version 400

in vec3 vert_texcoord;

uniform samplerCube CubeMap;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec3 normals;
layout (location = 2) out vec4 glow;
layout (location = 3) out vec4 background;

void main()
{
	vec3 frag_color = texture(CubeMap, vert_texcoord).xyz;

    diffuse = vec4(vec3(0), 0);
	normals = vec3(0);
	glow = vec4(vec3(0), 0.0f);
	background = vec4(frag_color, 1.0f);
}