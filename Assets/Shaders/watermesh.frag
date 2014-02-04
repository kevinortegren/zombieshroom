#version 410 core 

in vec3 WorldPos_FS_in;                                                                        
in vec2 TexCoord_FS_in;                                                                        

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec2 normals;
layout (location = 2) out vec4 glow;
layout (location = 3) out vec4 background;

uniform sampler2D g_Specular;
uniform samplerCube g_CubeMap;
uniform sampler2D g_LA;
uniform sampler2D g_Normal;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};  

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
	mat4 normalMatrix;
	vec3 gEyeWorldPos;
};    

float Fresnel(float p_dot, float p_fresnelBias, float p_fresnelPow)
{
	float facing = (1.0f - p_dot);
	return max(p_fresnelBias + (1.0f - p_fresnelBias) * pow(facing, p_fresnelPow), 0.0f);
}

void main()
{
	//Calculate normals
    vec3 normalMap			= texture(g_Normal, TexCoord_FS_in).rgb;
	normalMap				= normalize(normalMap.xyz*2-1); 
	vec3 viewNormal			= normalize(viewMatrix*vec4(normalMap,0.0f)).rgb;

	//Calculate transparent color and refraction
	vec2 screenTexCoord		= gl_FragCoord.xy / textureSize(g_LA, 0);
    vec3 refractionColor	= texture(g_LA, screenTexCoord + normalMap.xz * 0.2f).rgb;

	//Reflection calculations
	vec3 incidentW			= WorldPos_FS_in - gEyeWorldPos;
	vec3 refW				= reflect(incidentW, normalMap);
	vec3 reflectionColor	= texture(g_CubeMap, refW).rgb;

	//Calculate fresnel factor
	float ndotl		= max(dot(normalize(gEyeWorldPos - WorldPos_FS_in), normalMap), 0.0f);
	float fresnel	= Fresnel(ndotl, 0.2f, 5.0f);

	//Diffuse water color
	vec3 diffuseWaterColor  = texture(g_Specular, TexCoord_FS_in).rgb; 

	//Lerp refraction and reflection color
	vec3 refracReflect = mix(refractionColor, reflectionColor, fresnel);

	//Lerp water color and refraction+reflection color
	vec3 finalColor	= mix(refracReflect, diffuseWaterColor, 0.3f);

	//Calculate result color
	vec4 result				= vec4(finalColor, 1.0f);

	//Outputs
	diffuse					= vec4(0.0f, 0.0f, 0.0f, 0.6f);
	normals					= vec2(viewNormal.xy);
	glow					= vec4(0.0f, 0.0f, 0.0f, 0.0f);
	background				= result;
}