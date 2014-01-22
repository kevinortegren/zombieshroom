#version 400

in vec3 vert_normal;
in vec2 vert_texcoord;
in vec4 view;
in vec3 vert_tangent;
in vec3 vert_bitangent;

uniform sampler2D g_Diffuse;
uniform sampler2D g_Specular;
uniform sampler2D g_Normal;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec3 normals;

void main()
{
	float specTerm = texture(g_Specular, vert_texcoord).r;
	vec3 frag_color = texture(g_Diffuse, vert_texcoord).xyz;
	
	vec3 normalT = texture(g_Normal, vert_texcoord).xyz;
	normalT = normalT * 2.0f - 1.0f;

	mat3 TBNmatrix = mat3(vert_tangent, vert_bitangent, vert_normal);

	vec3 bumpNormal = normalize(TBNmatrix * normalT);

	diffuse = vec4(frag_color.xyz, specTerm);
	normals = vec3(bumpNormal * 0.5 + 0.5);
}