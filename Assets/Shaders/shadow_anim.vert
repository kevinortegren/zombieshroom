#version 400

layout(location = 0) in vec3 in_position;
layout(location = 5) in uvec4 in_boneID;
layout(location = 6) in vec4 in_weights;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
	mat4 bones[20];
};

void main()
{
	mat4 boneTransform = bones[in_boneID.x] * in_weights.x;
	boneTransform     += bones[in_boneID.y] * in_weights.y;
	boneTransform     += bones[in_boneID.z] * in_weights.z;
	boneTransform     += bones[in_boneID.w] * in_weights.w;

	vec4 pos = boneTransform * vec4( in_position, 1.0f );

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * pos;
}
