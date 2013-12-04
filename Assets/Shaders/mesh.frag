#version 400

in vec3 vert_normal;
in vec2 vert_texcoord;
in vec4 view;

/*// Ambient
uniform vec3 coefficientAmbient;
uniform vec3 intensityAmbient;

// Diffuse
uniform vec3 coefficientDiffuse; 

// Specular
uniform vec3 coefficientSpecular;
uniform float powerSpecular;
uniform vec3 intensitySpecular;*/

uniform sampler2D g_Diffuse;
uniform sampler2D g_Specular;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec3 normals;

void main()
{
	float specTerm = texture(g_Specular, vert_texcoord).r;
	vec3 frag_color = texture(g_Diffuse, vert_texcoord).xyz;
	vec3 normal = normalize(vert_normal);	

	diffuse = vec4(frag_color, specTerm);
	normals = vec3(normal * 0.5 + 0.5);
}