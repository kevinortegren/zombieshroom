#version 400

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

uniform mat3 normalMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 lightDirection;

uniform vec3 intensityDiffuse;

out vec3 vert_normal;

out vec4 vert_eyeCoords;
out vec3 vert_lightVec;

out vec3 vert_diffuseColor;

void main()
{
	vert_normal = normalize( normalMatrix * in_normal );

	vert_eyeCoords = viewMatrix * modelMatrix * vec4( in_position, 1.0f );

	vert_lightVec = normalize( -lightDirection );
	vert_diffuseColor = max( 0.0f, dot( normalize( vert_lightVec ), vert_normal ) ) * intensityDiffuse;

	gl_Position = projectionMatrix * vert_eyeCoords;
}
