#version 400

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;
layout(location = 3) in vec3 in_tangent;
layout(location = 4) in vec3 in_bitangent;
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
	mat4 normalMatrix;
	mat4 bones[20];
};

out vec3 vert_normal;
out vec2 vert_texcoord;
out vec3 vert_tangent;
out vec3 vert_bitangent;
out vec4 view;

void main()
{
	mat4 boneTransform = bones[in_boneID.x] * in_weights.x;
    boneTransform     += bones[in_boneID.y] * in_weights.y;
    boneTransform     += bones[in_boneID.z] * in_weights.z;
    boneTransform     += bones[in_boneID.w] * in_weights.w;

	vec4 norm = boneTransform * vec4(in_normal, 0.0f);

	vert_normal = normalize( viewMatrix * normalMatrix * norm).xyz;
	vert_tangent = normalize( viewMatrix * normalMatrix * vec4(in_tangent, 0.0f)).xyz;
	vert_bitangent = normalize( viewMatrix * normalMatrix * vec4(in_bitangent, 0.0f)).xyz;

	vert_texcoord = in_texcoord;

	view = viewMatrix * modelMatrix * vec4( in_position, 1.0f );

	vec4 pos = boneTransform * vec4( in_position, 1.0f );

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * pos;

}
