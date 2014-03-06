#version 410

in vec2 vert_texCoord;
flat in int vert_textureNumber;

uniform sampler2D tile[15];

out vec4 frag_color;

void main()
{
	frag_color = texture(tile[vert_textureNumber], vert_texCoord);
}