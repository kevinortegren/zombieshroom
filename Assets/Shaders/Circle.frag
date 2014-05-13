#version 400

out vec4 frag_color;

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
	mat4 normalMatrix;
	vec4 color;
};

void main()
{
	frag_color = color;
}