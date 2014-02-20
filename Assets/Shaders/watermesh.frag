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
uniform sampler2D g_NormalMap;
uniform sampler2D g_SceneNormals;

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
	float gTime;
	vec4 gOptions;
};    

vec3 GetVSPositionFromDepth(float z, vec2 screenCoord)
{ 
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

vec3 convertCameraSpaceToScreenSpace(in vec3 p_cameraSpace)
{
	vec4 clipSpace = projectionMatrix * vec4(p_cameraSpace, 1);
	vec3 NDCSpace = clipSpace.xyz / clipSpace.w;
	vec3 screenSpace = 0.5 * NDCSpace + 0.5;
	return screenSpace;
}

float linearizeDepth(in float depth) {
	return projectionMatrix[3][2] / (depth - projectionMatrix[2][2]);
}

void main()
{
	float time = gTime/40.0;

	////////////////////////////////////////////////////////////////////////////
	//Normal mapping
	////////////////////////////////////////////////////////////////////////////
    vec3 normalMap			= texture(g_Normal, TexCoord_FS_in).rgb;
	normalMap				= normalize(normalMap.xyz*2-1); 
	if(gOptions.y == 0.0)
	{
		vec3 normal1 			= texture(g_NormalMap, TexCoord_FS_in * 64.0 + vec2(sin(time - 1.0) , time) ).xyz;
		vec3 normal2 			= texture(g_NormalMap, -TexCoord_FS_in * 32.0 + vec2(sin(time*0.5)+1.0 , time*0.5) ).xyz;
		vec3 normalT 			= mix(normal1, normal2, 0.5);
		normalT 				= normalize(normalT);
		vec3 tangent			= cross(vec3(0,1,0), normalMap);
		vec3 bitangent			= cross(tangent, normalMap);
		mat3 TBN				= mat3(tangent, bitangent, normalMap);
		normalMap 				= TBN * normalT;
	}
	vec3 viewNormal			= normalize(viewMatrix*vec4(normalMap,0.0f)).rgb;

	////////////////////////////////////////////////////////////////////////////
	//Calculate transparent color and refraction
	////////////////////////////////////////////////////////////////////////////
	vec2 screenTexCoord		= gl_FragCoord.xy / textureSize(g_LA, 0);
	ivec2 screenAbsCoord	= ivec2(gl_FragCoord.xy);
	ivec2 refractedUV		= clamp(screenAbsCoord + ivec2(normalMap.xz * 100.0f), ivec2(0), textureSize(g_LA, 0));
	float refractionDepth	= texelFetch(g_Depth, refractedUV, 0).r;
	vec3 refractionColor;
	//Check if refracted vector is hitting an object in the foreground
	if(refractionDepth > gl_FragCoord.z && gOptions.w == 0.0)
		refractionColor	= texelFetch(g_LA, refractedUV, 0).rgb;
	else
	{
		refractionColor	= texelFetch(g_LA, ivec2(gl_FragCoord.xy), 0).rgb;
		refractionDepth = texelFetch(g_Depth, ivec2(gl_FragCoord.xy), 0).r;
	}

	////////////////////////////////////////////////////////////////////////////
	//Cubemap reflections
	////////////////////////////////////////////////////////////////////////////
 	vec3 incidentW			= WorldPos_FS_in - gEyeWorldPos;
	vec3 refW				= reflect(incidentW, normalMap);
	refW.z = -refW.z;

	vec3 finalResult =  vec3(0.2, 0.4, 0.47);//texture(g_CubeMap, refW).rgb;

	////////////////////////////////////////////////////////////////////////////
	//Real-time Local Reflections using ray marching
	////////////////////////////////////////////////////////////////////////////
	if(gOptions.x == 0.0)
	{
	//Length of the reflection vector
	float initialStepAmount = 0.01;

	//Water fragment view space position
	vec3 posV 		= GetVSPositionFromDepth(gl_FragCoord.z, screenTexCoord);
	vec3 viewVec	= normalize(posV);
	//Viewspace vector reflected on the water normal
	vec3 refV		= normalize(reflect(viewVec, viewNormal));

	//if(refV.z > -0.5)
	//{
		//Water fragment screen space position
		vec3 posS 		= convertCameraSpaceToScreenSpace(posV);
		//View space position when adding viewspace reflection vector to view space water frag position
		vec3 posRefV	= posV + refV;
		//Convert position above to screen space
		vec3 posRefS	= convertCameraSpaceToScreenSpace(posRefV);
		//Finally get the screen space reflection vector and multiply stem amount, this vector is used for ray marching in screen space
		vec3 refS 		= initialStepAmount*normalize(posRefS - posS);

		//Set up start values for ray marching
		vec3 oldPosition 		= posS + refS;
		vec3 currentPosition 	= oldPosition + refS;
		vec2 firstOccludedPos;
		int firstOccludedCount;

		int count 					= 0;
		int loops					= 1000;
		int numRefinements 			= 0;
		float stepRefinementAmount 	= 0.7;
		int maxRefinements 			= 3;
		bool firstOccludedSample	= false;
		//Ray marching loop
		while(count < loops)
		{
			//Stop ray march when outside screen space
			if(currentPosition.x < 0 || currentPosition.x >= 1 ||
			   currentPosition.y < 0 || currentPosition.y >= 1 ||
			   currentPosition.z < 0 || currentPosition.z >= 1)
			{
				//firstOccludedSample = true;
				//firstOccludedCount 	= count;
				//firstOccludedPos	= oldPosition.xy;
				count = loops;
				break;
			}
			//Sample depth from G-buffer and compare to current ray position depth
			vec2 samplePos 		= currentPosition.xy;
			float currentDepth 	= linearizeDepth(currentPosition.z);
			float sampleDepth 	= linearizeDepth(texture(g_Depth, samplePos).x);
			float diff 			= currentDepth - sampleDepth;
			float error 		= length(refS);
			//Save first time the ray goes behind an object, this is used to fill in non-existent color data
			//if(diff >= 0 && firstOccludedSample == false)
			//{
			//	firstOccludedSample = true;
			//	firstOccludedCount 	= count;
			//	firstOccludedPos	= oldPosition.xy;
			//}
			//If ray is behind and object and in close proximity to it we have in intersection
			if(diff >= 0 && diff < error*0.05)
			{
				refS *= stepRefinementAmount;
				currentPosition = oldPosition;
				numRefinements++;
				if(numRefinements >= maxRefinements)
				{
					 /*vec2 normalAtPos = texture(g_SceneNormals, samplePos).xy;
					 vec3 normal;
					 normal.xy = normalAtPos.xy;
    				 normal.z = sqrt(1-dot(normal.xy, normal.xy));
					 float orientation = dot(refV, normal);
					 if(orientation < 0)
					 {
					 	float cosAngIncidence = -dot(viewVec, viewNormal);
					 	cosAngIncidence = clamp(1-cosAngIncidence,0.0,1.0);
					 	//Get the color and end the loop
						finalResult =  texture(g_LA, samplePos).rgb * (1.0 - float(count)/float(loops)) * cosAngIncidence;
					}*/
					finalResult =  texture(g_LA, samplePos).rgb * (1.0 - float(count)/float(loops));// * cosAngIncidence;
					break;
				}
			}

			//Step ray
			oldPosition 	= currentPosition;
			currentPosition = oldPosition + refS;
			count++;
		}
		//if(count == loops && firstOccludedSample == true)
		//{
			//if(texture(g_Depth, firstOccludedPos).r != 1)
			//finalResult = texture(g_LA, firstOccludedPos).rgb * (1.0 - float(firstOccludedCount)/float(loops));
		//}
	//}
	}
	//Mix reflection color with water color
	finalResult = mix(finalResult, vec3(0.2, 0.4, 0.47), 0.5);
 
	vec3 reflectionColor = finalResult;
	////////////////////////////////////////////////////////////////////////////
	//Calculate fresnel factor
	////////////////////////////////////////////////////////////////////////////
	float ndotl		= max(dot(normalize(gEyeWorldPos - WorldPos_FS_in), normalMap), 0.0f);
	float fresnel	= Fresnel(ndotl, 0.2f, 6.0f);

	////////////////////////////////////////////////////////////////////////////
	//Lerp water color and refraction color
	////////////////////////////////////////////////////////////////////////////
	float vdist = abs(GetVSPositionFromDepth(refractionDepth, screenTexCoord).z - GetVSPositionFromDepth(gl_FragCoord.z, screenTexCoord).z);
	float distFac = clamp(gOptions.z*vdist, 0.0f, 1.0f);
	
	vec3 waterColor	= mix(refractionColor, vec3(0f, 0.15f, 0.115f), distFac);

	////////////////////////////////////////////////////////////////////////////
	//Water foam
	////////////////////////////////////////////////////////////////////////////
	//vec3 foamWaterColor  = texture(g_Specular, TexCoord_FS_in*256.0f).rgb;  calculations
	//float foamDistFac = clamp(0.05f*vdist, 0.0f, 1.0f);
	//waterColor = mix(foamWaterColor, waterColor, foamDistFac);

	////////////////////////////////////////////////////////////////////////////
	//Calculate result color
	////////////////////////////////////////////////////////////////////////////
	vec4 result				= vec4(mix(waterColor, reflectionColor, fresnel), 1.0f);

	////////////////////////////////////////////////////////////////////////////
	//Outputs
	////////////////////////////////////////////////////////////////////////////
	diffuse					= vec4(0.0f, 0.0f, 0.0f, 0.6f);
    
    float p = sqrt(viewNormal.z*8+8);
    normals = viewNormal.xy/p + 0.5;

	glow					= vec4(0.0f, 0.0f, 0.0f, 0.0f);
	background				= result;
}