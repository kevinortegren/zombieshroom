#version 400

in vec2 vert_texCoord;

uniform sampler2D g_Input;
uniform sampler2D g_Glow;

out vec4 frag_color;

void main()
{
    vec4 input = texture(g_Input, vert_texCoord);
    
    input.g = 0;
    
	frag_color = input;
}