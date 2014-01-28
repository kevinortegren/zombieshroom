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

float Gaussian (float x, float deviation)
{
	return (1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation)));	
}

void main()
{
    vec2 TexelCoord = gl_FragCoord.xy / textureSize(g_Scene, 0);

    // Inverse size of Blur Sampler.
    float dx = 1.0f / textureSize(g_Input, 0).x;

    vec4 blur = texture(g_Input, TexelCoord) * g_Weights[0];
    vec4 glow = texture(g_Glow, TexelCoord);
    
    for( int i = 1; i < 10; i++ )
	{
		blur += texture(g_Input, TexelCoord + vec2( 0.0, PixelOffset[i] ) * dx ) * g_Weights[i];
		blur += texture(g_Input, TexelCoord - vec2( 0.0, PixelOffset[i] ) * dx ) * g_Weights[i];
	}
    
	frag_color = texture(g_Scene, TexelCoord) + blur;
}