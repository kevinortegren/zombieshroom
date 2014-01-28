#version 400

in vec3 vert_normal;
in vec2 vert_texcoord;
in vec4 view;

uniform sampler2D g_Diffuse;
uniform sampler2D g_Specular;
uniform sampler2D g_Glow;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec3 normals;
layout (location = 2) out vec4 glow;

void main()
{
	float specTerm = texture(g_Specular, vert_texcoord).r;
	vec3 diffuse_color = texture(g_Diffuse, vert_texcoord).xyz;
	vec3 glow_color = texture(g_Glow, vert_texcoord).xyz;

	vec3 normal = normalize(vert_normal);	

	diffuse = vec4(vec3(0.0f), specTerm);
	normals = vec3(normal * 0.5 + 0.5);
	glow = vec4(vec3(1,0,0), 1.0f);
}