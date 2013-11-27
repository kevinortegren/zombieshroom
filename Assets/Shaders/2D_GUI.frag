#version 400

in vec4 vert_color;
in vec2 vert_texCoord;

uniform sampler2D texSampler;

out vec4 frag_color;

void main()
{
	frag_color = vert_color * texture(texSampler, vert_texCoord);
}