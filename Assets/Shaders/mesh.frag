#version 400

in vec3 vert_normal;
in vec2 vert_texcoord;
in vec4 view;

uniform sampler2D g_Diffuse;
uniform sampler2D g_Specular;
uniform sampler2D g_Glow;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec2 normals;
layout (location = 2) out vec4 glow;

void main()
{
	float specTerm = texture(g_Specular, vert_texcoord).r;
	vec3 diffuse_color = texture(g_Diffuse, vert_texcoord).xyz;
	vec3 glow_color = texture(g_Glow, vert_texcoord).xyz;

	vec3 normal = normalize(vert_normal);	

	diffuse = vec4(diffuse_color, specTerm);
	normals = normal.xy;
	glow = vec4(vec3(glow_color.xyz), 0.0f);
}