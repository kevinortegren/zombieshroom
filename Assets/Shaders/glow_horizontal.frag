#version 430

#define TEXTURE_TAPS 5

in vec2 vert_texCoord;

// Uniforms
layout(std140) uniform PerTech
{
    float g_Width;
	float g_Height;
	float g_BlurRadius;
    //float g_Gauss[11];
};

// Textures
uniform sampler2D g_Glow;

// Constants
const float PixelOffset[10] =
    float[10]( 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 );

const float g_Gauss[11] = float[11] 
(
0.009292,
0.126648,
0.122629,
0.111321,
0.094743,
0.075597,
0.056552,
0.039663,
0.026080,
0.016077,
0.009292
);
    
out vec4 frag_color;

float Gaussian (float x, float deviation)
{
	return (1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation)));	
}

void main()
{
    if(g_BlurRadius == 0.0)
    {
        discard;
    }
    else
    {
        /*float deviation = g_BlurFactor * 0.35;
        deviation *= deviation;
        float strength = 1.0 - g_BlurStrength;*/
        
        vec2 TexelCoord = gl_FragCoord.xy / vec2(g_Width, g_Height);	

        // Inverse size of Glow Sampler.
        float dx = 1.0f / textureSize(g_Glow, 0).x;

        vec4 blur = texture(g_Glow, TexelCoord) * g_Gauss[0];

        for( int i = 1; i < TEXTURE_TAPS; i++ )
        {
            blur += texture(g_Glow, TexelCoord + vec2( PixelOffset[i], 0.0 ) * dx * g_BlurRadius) * g_Gauss[i + 1];
            blur += texture(g_Glow, TexelCoord - vec2( PixelOffset[i], 0.0 ) * dx * g_BlurRadius) * g_Gauss[i + 1];
        }
        
        blur.w = 1.0;
        frag_color = clamp(blur, 0.0, 1.0);
    }
}