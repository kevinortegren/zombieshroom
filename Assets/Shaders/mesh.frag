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

uniform sampler2D diffuse;

layout (location = 0) out vec3 def_p;
layout (location = 1) out vec3 def_n;
layout (location = 2) out vec4 def_a;


void main()
{
	vec3 frag_color = texture(diffuse, vert_texcoord).xyz;
	vec3 normal = normalize(vert_normal);	

	def_p = frag_color;
	def_n = vec3(normal * 0.5 + 0.5);
	def_a = view;
}