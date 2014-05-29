#version 400

layout(location = 0) in vec3 in_position;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
};

out vec3 vert_texcoord;

void main()
{
	vec3 pos = (projectionMatrix * viewMatrix * modelMatrix * vec4( in_position, 1.0 )).xyz;

	gl_Position = vec4(pos.xyz, pos.z + 0.001);

	vert_texcoord = in_position.xyz;	
}
