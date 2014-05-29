#version 430

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 invView;
	mat4 invProj;
	mat4 invViewProj;
};

in vec2 gs_TexCoord;
in vec2 gs_ScreenCoord;
in float gs_Depth;
in vec4 gs_color;

uniform sampler2D g_Diffuse;
uniform sampler2D g_Depth;

out vec4 out_color;

vec3 GetVSPositionFromDepth(vec2 texcoord)
{
	float z = texture(g_Depth, texcoord).r;

	z = z * 2 - 1;

	float x = texcoord.x * 2 - 1;
	float y = texcoord.y * 2 - 1;

	vec4 vProjectedPos = vec4(x, y, z, 1.0f);
	vec4 sPos = invProj * vProjectedPos;

	return (sPos.xyz / sPos.w);
}

void main(void) {

    vec2 TexCoord = gl_FragCoord.xy / textureSize(g_Depth, 0);   

	vec4 frag_color = texture(g_Diffuse, gs_TexCoord);

    float depth = gs_Depth * -1.0f;   
    float depthViewSample = GetVSPositionFromDepth(TexCoord).z * -1.0f;

    float depthDiff = depthViewSample - depth;
    
	depthDiff = clamp(depthDiff * 0.1f, 0.0f, 1.0f);

	out_color = vec4(frag_color.xyz, frag_color.a * depthDiff) * gs_color;
}