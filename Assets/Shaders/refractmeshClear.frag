#version 400

in vec3 vert_normal;
in vec2 vert_texcoord;
in vec3 vert_tangent;
in vec3 vert_bitangent;                                                                      

out vec4 out_color;

uniform sampler2D g_LA;
uniform sampler2D g_Depth;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 invView;
	mat4 invProj;
	mat4 invViewProj;
};  

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
	mat4 normalMatrix;
};    

void main()
{
	////////////////////////////////////////////////////////////////////////////
	//Calculate transparent color and refraction - Need more accuracy when refracting using view space normals
	////////////////////////////////////////////////////////////////////////////
	vec2 screenTexCoord		= gl_FragCoord.xy / textureSize(g_LA, 0);
	ivec2 screenAbsCoord	= ivec2(gl_FragCoord.xy);
	ivec2 refractedUV		= clamp(screenAbsCoord + ivec2(vec2(vert_normal.x, vert_normal.y) * 30.0f), ivec2(0), textureSize(g_LA, 0));
	float refractionDepth	= texelFetch(g_Depth, refractedUV, 0).r;

	vec3 refractionColor;
	//Check if refracted vector is hitting an object in the foreground
	if(refractionDepth > gl_FragCoord.z)
		refractionColor	= texelFetch(g_LA, refractedUV, 0).rgb;
	else
		refractionColor	= texelFetch(g_LA, ivec2(gl_FragCoord.xy), 0).rgb;

	////////////////////////////////////////////////////////////////////////////
	//Outputs
	////////////////////////////////////////////////////////////////////////////
    out_color = vec4(refractionColor, 1.0);
}