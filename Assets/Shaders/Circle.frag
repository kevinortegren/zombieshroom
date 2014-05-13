#version 400

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
};

void main()
{
	vec4 posView = invProjection * gl_FragCoord;	
	vec4 center = viewMatrix * vec4(0,0,0,1);
	
	if(posView.z < center.z)
		discard;
	
	frag_color = color;
}