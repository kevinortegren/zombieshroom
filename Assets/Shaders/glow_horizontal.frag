#version 400

in vec2 vert_texCoord;

// Uniforms
layout(std140) uniform PerTech
{
    int g_Width;
	int g_Height;
	float g_BlurFactor;
	float g_BlurStrength;
	float g_BlurRadius;
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
	float blurS = 2.2f;
    float blurF = 2.0f;

    float deviation = g_BlurFactor * 0.35;
	deviation *= deviation;
	float strength = 1.0 - g_BlurStrength;
    
    vec2 TexelCoord = gl_FragCoord.xy / vec2(g_Width, g_Height);	

    // Inverse size of Glow Sampler.
    float dy = 1.0f / textureSize(g_Glow, 0).y;

    vec4 blur = texture(g_Glow, TexelCoord) * Gaussian(blurF * strength, deviation);

    for( int i = 1; i < 10; i++ )
	{
		blur += texture(g_Glow, TexelCoord + vec2( 0.0, PixelOffset[i] ) * dy * g_BlurRadius) * Gaussian(i * strength, deviation);
		blur += texture(g_Glow, TexelCoord - vec2( 0.0, PixelOffset[i] ) * dy * g_BlurRadius) * Gaussian(i * strength, deviation);
	}
    
    blur.w = 1.0;
	frag_color = clamp(blur, 0.0, 1.0);
}