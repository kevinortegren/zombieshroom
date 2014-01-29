#version 400

in vec2 vert_texcoord;

out vec4 out_Color;

uniform sampler2D g_Specular;

void main()
{
	vec4 color = texture(g_Specular, vert_texcoord);
	vec4 finalcolor = vec4(color.r, color.g, color.b, 0.5f);
	out_Color = finalcolor;
	
}