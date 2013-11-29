#version 400

out vec3 frag_color;

in Fragment
{
	vec3 color;
} fragment;

void main()
{
	frag_color = vec3(0.3 + fragment.color);
}