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
	vec3 gEyeWorldPos;
};    

vec3 GetVSPositionFromDepth(float z, vec2 screenCoord)
{
	if(z == 1)
	discard;
  
	z = z * 2 - 1;

	float x = screenCoord.x * 2 - 1;
	float y = screenCoord.y * 2 - 1;

	vec4 vProjectedPos = vec4(x, y, z, 1.0f);
	vec4 sPos = invProj * vProjectedPos;

	return (sPos.xyz / sPos.w);
}

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
	vec2 refractedUV		= clamp(screenTexCoord + normalMap.xz * 0.15f, vec2(0.0f), vec2(1.0f));
	float refractionDepth	= texture(g_Depth, refractedUV).r;
	vec3 refractionColor;
	if(refractionDepth > gl_FragCoord.z)
		refractionColor	= texture(g_LA, refractedUV).rgb;
	else
	{
		refractionColor	= texture(g_LA, screenTexCoord).rgb;
		refractionDepth = texture(g_Depth, screenTexCoord).r;
	}

	//Reflection calculations
	vec3 incidentW			= WorldPos_FS_in - gEyeWorldPos;
	vec3 refW				= reflect(incidentW, normalMap);
	vec3 reflectionColor	= texture(g_CubeMap, refW).rgb;

	//Calculate fresnel factor
	float ndotl		= max(dot(normalize(gEyeWorldPos - WorldPos_FS_in), normalMap), 0.0f);
	float fresnel	= Fresnel(ndotl, 0.2f, 5.0f);

	//Diffuse water color
	vec3 diffuseWaterColor  = texture(g_Specular, TexCoord_FS_in).rgb; 

	//Lerp water color and refraction color
	float distFac = clamp(0.01f*abs(GetVSPositionFromDepth(refractionDepth, screenTexCoord).z - GetVSPositionFromDepth(gl_FragCoord.z, screenTexCoord).z), 0.0f, 1.0f);
	vec3 waterColor	= mix(refractionColor, vec3(0f, 0.15f, 0.115f), distFac);

	//Calculate result color
	vec4 result				= vec4(mix(waterColor, reflectionColor, fresnel), 1.0f);

	//Outputs
	diffuse					= vec4(0.0f, 0.0f, 0.0f, 0.6f);
	normals					= vec2(viewNormal.xy);
	glow					= vec4(0.0f, 0.0f, 0.0f, 0.0f);
	background				= result;
}