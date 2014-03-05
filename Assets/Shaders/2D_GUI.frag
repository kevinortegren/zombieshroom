#version 400

in vec3 vert_texCoord;

uniform sampler2D tile[8];

uniform int outputHeight;
uniform int tileSize;

out vec4 frag_color;

void main()
{
  int texIndex = int(floor(vert_texCoord.z));
	frag_color = texture(tile[texIndex], vec2(vert_texCoord.x, vert_texCoord.y * outputHeight / float(tileSize) - texIndex));
}