#version 400

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_texCoord;

out vec2 vert_texCoord;

void main()
{
	vert_texCoord = in_texCoord;
	gl_Position = vec4( in_position, 0.0, 1.0);
}
