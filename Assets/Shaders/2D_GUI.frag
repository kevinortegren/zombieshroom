#version 430

in vec2 vert_texCoord;
in float vert_textureNumber;

uniform sampler2D tile[32];

out vec4 frag_color;

void main()
{
	frag_color = texture(tile[int(floor(vert_textureNumber))], vert_texCoord);
}