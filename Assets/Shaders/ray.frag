#version 400

uniform sampler2D g_Diffuse;

out vec4 frag_color;

in vec2 texCoord;

void main()
{
	
	frag_color = texture(g_Diffuse, texCoord.st).rgba;
}