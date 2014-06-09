#version 430

in vec2 ex_TexCoord;
out vec4 out_Color;

uniform sampler2D g_Diffuse;
uniform sampler2D g_SSAO;

const int blurSize = 4;

layout(std140) uniform Lights
{
	vec4 g_Ambient;
};

void main() {
	vec2 texelSize = 1.0 / vec2(textureSize(g_SSAO, 0));
	float result = 0.0;
	vec2 hlim = vec2(float(-blurSize) * 0.5 + 0.5);
	for (int i = 0; i < blurSize; ++i) 
	{
		for (int j = 0; j < blurSize; ++j) 
		{
			vec2 offset = (hlim + vec2(float(i), float(j))) * texelSize;
			result += texture(g_SSAO, ex_TexCoord + offset).r;
		}
	}
 
	result = result / float(blurSize * blurSize);

	out_Color = vec4(g_Ambient.rgb * result * texture(g_Diffuse, ex_TexCoord.st).rgb, 1.0);
}