#version 400

in vec3 vert_normal;

/*// Ambient
uniform vec3 coefficientAmbient;
uniform vec3 intensityAmbient;

// Diffuse
uniform vec3 coefficientDiffuse; 

// Specular
uniform vec3 coefficientSpecular;
uniform float powerSpecular;
uniform vec3 intensitySpecular;*/

layout (location = 0) out vec3 def_p;
layout (location = 1) out vec3 def_n;

void main()
{
	vec3 frag_color = vec3(1);
	vec3 normal = normalize(vert_normal);	

	def_p = frag_color;
	def_n = vec3(normal * 0.5 + 0.5);
}