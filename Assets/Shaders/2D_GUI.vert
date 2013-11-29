#version 400

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec2 in_texCoord;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec4 vert_color;
out vec2 vert_texCoord;

void main()
{
	vert_color = in_color;
	vert_texCoord = in_texCoord;
	gl_Position = projectionMatrix * modelMatrix * vec4( in_position, 0.0f, 1.0f);
}
