#version 400

uniform sampler2D diffuseMap;

in Fragment
{
	vec2 texCoord;
} fragment;

out vec3 frag_color;

void main()
{
	frag_color = texture(diffuseMap, fragment.texCoord.st).rgb;
}