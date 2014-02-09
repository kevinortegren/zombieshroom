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
	ivec2 screenAbsCoord	= ivec2(gl_FragCoord.xy);
	ivec2 refractedUV		= clamp(screenAbsCoord + ivec2(normalMap.xz * 100.0f), ivec2(0), textureSize(g_LA, 0));
	float refractionDepth	= texelFetch(g_Depth, refractedUV, 0).r;
	vec3 refractionColor;
	if(refractionDepth > gl_FragCoord.z)
		refractionColor	= texelFetch(g_LA, refractedUV, 0).rgb;
	else
	{
		refractionColor	= texelFetch(g_LA, ivec2(gl_FragCoord.xy), 0).rgb;
		refractionDepth = texelFetch(g_Depth, ivec2(gl_FragCoord.xy), 0).r;
	}

	//Reflection calculations
	vec3 posV				= (viewMatrix * vec4(gEyeWorldPos, 1.0f)).xyz;
	vec3 refV				= normalize(reflect(normalize(posV), viewNormal));
	vec3 posRefV			= posV + refV;
	vec3 posRefS			= (projectionMatrix * vec4(posRefV, 1.0f)).xyz / posRefV.z;
	vec3 refS 				= posRefS - vec3(screenTexCoord, (gl_FragCoord.z*2-1)/gl_FragCoord.w;
	float scalefactor		= 0.00139f / length(refS.xy);
	refS					*= scalefactor;
	vec3 currOffsetS		= vec3(screenTexCoord, (gl_FragCoord.z*2-1)/gl_FragCoord.w) + refS;
	currOffsetS.xy 			= vec2(currOffsetS.x * 0.5 + 0.5, currOffsetS.y * 0.5 + 0.5);
   	vec3 lastOffsetS 		= vec3(screenTexCoord, (gl_FragCoord.z*2-1)/gl_FragCoord.w);
   	lastOffsetS.xy 			= vec2(lastOffsetS.x * 0.5 + 0.5, lastOffsetS.y * 0.5 + 0.5);
   	refS 					= vec3(refS.x * 0.5 , refS.y * 0.5, refS.z);
   	int numSamples			= 1000;
   	int ncurrSample			= 0;

   	vec3 incidentW			= WorldPos_FS_in - gEyeWorldPos;
	vec3 refW				= reflect(incidentW, normalMap);
	refW.z = -refW.z;
   	vec3 finalResult 		= texture(g_CubeMap, refW).rgb;

   	float currSample;

   	while(ncurrSample < numSamples)
   	{
   		currSample = texture(g_Depth, currOffsetS.xy).r;
   		if(currSample < currOffsetS.z)
   		{
   			finalResult = texture(g_LA, currOffsetS.xy).rgb;
   			ncurrSample = numSamples + 1;
   		}
   		else
   		{
   			++ncurrSample;
   			currOffsetS += refS;
   		}
   	}

	vec3 reflectionColor = finalResult;

	//Calculate fresnel factor
	float ndotl		= max(dot(normalize(gEyeWorldPos - WorldPos_FS_in), normalMap), 0.0f);
	float fresnel	= Fresnel(ndotl, 0.2f, 5.0f);

	//Diffuse water color
	//vec3 foamWaterColor  = texture(g_Specular, TexCoord_FS_in*256.0f).rgb; 

	//Lerp water color and refraction color
	float vdist = abs(GetVSPositionFromDepth(refractionDepth, screenTexCoord).z - GetVSPositionFromDepth(gl_FragCoord.z, screenTexCoord).z);
	float distFac = clamp(0.01f*vdist, 0.0f, 1.0f);
	vec3 waterColor	= mix(refractionColor, vec3(0f, 0.15f, 0.115f), distFac);

	//Water foam calculations
	//float foamDistFac = clamp(0.05f*vdist, 0.0f, 1.0f);
	//waterColor = mix(foamWaterColor, waterColor, foamDistFac);

	//Calculate result color
	vec4 result				= vec4(mix(waterColor, reflectionColor, fresnel), 1.0f);

	//Outputs
	diffuse					= vec4(0.0f, 0.0f, 0.0f, 0.6f);
	normals					= vec2(viewNormal.xy);
	glow					= vec4(0.0f, 0.0f, 0.0f, 0.0f);
	background				= result;
}