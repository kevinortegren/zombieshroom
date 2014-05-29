#version 430

in vec4 vert_color;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec3 normals;
layout (location = 2) out vec4 glow;
layout (location = 3) out vec4 background;

void main()
{
	diffuse = vec4(0);
	normals = vec3(0);
	glow = vec4(0.0, 0.0, 0.0, 0.0);
	background = vert_color;
}