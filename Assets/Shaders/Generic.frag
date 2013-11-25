#version 400

in vec3 vert_normal;
in vec3 vert_lightVec;
in vec3 vert_eyeCoords;
in vec3 vert_diffuseColor;

/*// Ambient
uniform vec3 coefficientAmbient;
uniform vec3 intensityAmbient;

// Diffuse
uniform vec3 coefficientDiffuse; 

// Specular
uniform vec3 coefficientSpecular;
uniform float powerSpecular;
uniform vec3 intensitySpecular;*/

out vec3 frag_color;

void main()
{
	/*vec3 n = normalize( vert_normal );
	vec3 v = normalize( -vec3( vert_eyeCoords ) );
	vec3 r = reflect( -normalize( vert_lightVec ), vert_normal );
	
	vec3 specular = pow ( max( 0.0f, dot( r, v ) ), powerSpecular ) * intensitySpecular;

	frag_color = vec3( coefficientAmbient * intensityAmbient + coefficientDiffuse * vert_diffuseColor + coefficientSpecular * specular );*/
	frag_color = vec3(0.3 + vert_diffuseColor);
	//frag_color = vec3(1);
}