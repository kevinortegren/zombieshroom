#version 400

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140) uniform PerObject
{
    mat4 modelMatrix;
	mat4 normalMatrix;
};

out vec4 vert_normal;

void main()
{
	vert_normal = normalize( normalMatrix * vec4(in_normal, 0.0f));

	//vert_lightVec = normalize( -lightDirection );
	//vert_diffuseColor = max( 0.0f, dot( normalize( vert_lightVec ), vert_normal ) ) * intensityDiffuse;

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( in_position, 1.0f );
}
