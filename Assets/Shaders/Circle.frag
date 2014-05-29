#version 430

out vec4 frag_color;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 invView;
	mat4 invProjection;
};

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
	mat4 normalMatrix;
	vec4 color;
	vec2 viewport;
};

in float viewZ;

void main()
{
	vec4 center = viewMatrix * modelMatrix * vec4(0,0,0,1);
	
	if(viewZ < center.z)
		discard;
	
	frag_color = color;
}