#version 400

uniform sampler2D g_Diffuse;

out vec4 frag_color;

in vec2 texCoord;

void main()
{
	
	frag_color = vec4(texture(g_Diffuse, texCoord.st).rgb * 0.1, 1.0);
}