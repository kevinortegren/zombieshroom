#version 400

in vec2 vert_texcoord;
in vec4 view;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec3 normals;
layout (location = 2) out vec4 glow;

void main()
{
	diffuse = vec4(0,0,1,1);
	normals = vec3(0,1,0);
	glow = vec4(0,0,0, 1.0f);
}