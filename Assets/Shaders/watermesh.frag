#version 400

in vec2 vert_texcoord;

out vec4 out_Color;

uniform sampler2D g_Diffuse;

void main()
{
	float frag_color = texture2D(g_Diffuse, vert_texcoord).r;
	out_Color = vec4(frag_color,0,0, 1.0f);
	
}