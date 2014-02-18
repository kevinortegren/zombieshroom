#version 400

in vec3 vert_normal;
in vec2 vert_texcoord;
in vec4 view;
in vec3 vert_tangent;
in vec3 vert_bitangent;

uniform sampler2D g_Diffuse;
uniform sampler2D g_Specular;
uniform sampler2D g_Normal;
uniform sampler2D g_Glow;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec2 normals;
layout (location = 2) out vec4 glow;

void main()
{
	float specTerm = texture(g_Specular, vert_texcoord).r;
	vec3 frag_color = texture(g_Diffuse, vert_texcoord).xyz;
	vec3 glow_color = texture(g_Glow, vert_texcoord).xyz;
	

	vec3 normalT = texture(g_Normal, vert_texcoord).xyz;
	normalT = normalT * 2.0f - 1.0f;

	mat3 TBNmatrix = mat3(vert_tangent, vert_bitangent, vert_normal);

	vec3 bumpNormal = normalize(TBNmatrix * normalT);

	diffuse = vec4(frag_color.xyz, specTerm);
    float p = sqrt(bumpNormal.z*8+8);
    normals = bumpNormal.xy/p + 0.5;
	glow = vec4(vec3(glow_color.xyz), 0.0f);
}