#version 400

in vec2 vert_texCoord;

// Uniforms
layout(std140) uniform PerTech
{
    float g_Weights[10];
};

// Textures
uniform sampler2D g_Scene;
uniform sampler2D g_Glow;
uniform sampler2D g_Input;

// Constants
const float PixelOffset[10] =
    float[10]( 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 );

out vec4 frag_color;

void main()
{
    float dy = 1.0f / textureSize(g_Input, 0).y;

    vec4 blur = texture(g_Input, vert_texCoord) * g_Weights[0];
   
    for( int i = 1; i < 10; i++ )
	{
		blur += texture(g_Input, vert_texCoord + vec2( 0.0, PixelOffset[i] ) * dy ) * g_Weights[i];
		blur += texture(g_Input, vert_texCoord - vec2( 0.0, PixelOffset[i] ) * dy ) * g_Weights[i];
	}
    
    vec4 scene = texture(g_Scene, vert_texCoord);
    vec4 glow = texture(g_Glow, vert_texCoord);
    
	frag_color = scene + blur;
}