#version 410

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_texCoord;
layout(location = 2) in float in_textureNumber;

out vec2 vert_texCoord;
out float vert_textureNumber;

void main()
{
	vert_texCoord = in_texCoord;
  vert_textureNumber = in_textureNumber;
	gl_Position = vec4( in_position, 0.0f, 1.0f);
}
