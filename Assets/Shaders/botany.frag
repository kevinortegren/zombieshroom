#version 400

in vec3 vert_normal;
in vec2 vert_texcoord;

uniform sampler2D g_Diffuse;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec2 normals;
layout (location = 2) out vec4 glow;

void main()
{
    vec4 diffuse = texture(g_Diffuse, vert_texcoord);
	vec3 normal = normalize(vert_normal);	

	diffuse = vec4(diffuse.xyz, 0);
	normals = normal.xy;
	glow = vec4(vec3(0), 0.0f);
}