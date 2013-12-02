#version 400

out vec4 frag_color;

in Fragment
{
	vec4 color;
} fragment;

void main()
{
	frag_color = fragment.color;
}