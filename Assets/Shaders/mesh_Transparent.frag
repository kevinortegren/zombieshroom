#version 400

in vec3 vert_normal;
in vec2 vert_texcoord;
in vec4 view;

uniform sampler2D g_Diffuse;
uniform sampler2D g_Glow;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec2 normals;
layout (location = 2) out vec4 glow;

void main()
{
	if(texture(g_Diffuse, vert_texcoord).a < 0.5)
		discard;

	float specTerm = texture(g_Specular, vert_texcoord).r;
	vec3 frag_color = texture(g_Diffuse, vert_texcoord).xyz;
	vec3 glow_color = texture(g_Glow, vert_texcoord).xyz;

	mat3 TBNmatrix = mat3(vert_tangent, vert_bitangent, vert_normal);

	diffuse = vec4(frag_color, specTerm);
    float p = sqrt(bumpNormal.z*8+8);
	glow = vec4(vec3(glow_color.xyz), trans);
}