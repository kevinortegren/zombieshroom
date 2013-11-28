#version 400

in vec2 vert_texCoord;

uniform sampler2D texSampler;

out vec4 frag_color;

void main()
{
	frag_color = vec4(1);//texture(texSampler, vert_texCoord);
}