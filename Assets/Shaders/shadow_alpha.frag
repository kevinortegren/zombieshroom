#version 430

in vec2 vert_texcoord;

uniform sampler2D g_Diffuse;

void main()
{
	float alpha = texture(g_Diffuse, vert_texcoord).a;
	if(alpha >= 0.5)
	{
		discard;
	}
}