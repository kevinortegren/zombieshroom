#version 400

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
	mat4 shadowCasterViewProjectionMatrix[4];
};

struct DirectionalLight
{
	vec3 LightDirection;
	vec4 Color;
};

in vec2 ex_TexCoord;
in DirectionalLight ex_Light;
in vec3 sunPosition;

// GBuffer.
uniform sampler2D g_Diffuse;
uniform sampler2D g_Normals;
uniform sampler2D g_Depth;
uniform sampler2DArrayShadow g_ShadowDepthPCF;
uniform sampler2DArray g_ShadowDepth;

out vec4 out_Color;

const int PCF_NUM_SAMPLES = 16;

vec2 poissonDisk[16] = vec2[](
	vec2( 0.2134463,	 0.4247225),
	vec2( 0.4785358,	-0.3093533),
	vec2( 0.6953664,	 0.6155604),
	vec2(-0.3491541,	 0.7737809),
	vec2(-0.3364094,	 0.110289f),
	vec2(-0.2225659,	-0.6537814),
	vec2( 0.1859272,	-0.9121895),
	vec2( 0.6556994,	 0.1956618),
	vec2( 0.3703193,	 0.8869988),
	vec2( 0.1125489,	-0.09327701),
	vec2( 0.9311103,	-0.348781),
	vec2( 0.6107655,	-0.787773),
	vec2(-0.8762138,	-0.2776574),
	vec2(-0.6795356,	-0.6391076),
	vec2(-0.5959228,	 0.4477699),
	vec2(-0.4264759,	-0.2957492)

);

vec3 GetVSPositionFromDepth()
{
	float z = texture(g_Depth, ex_TexCoord).r;

	if(z == 1)
	discard;
  
	z = z * 2 - 1;

	float x = ex_TexCoord.x * 2 - 1;
	float y = ex_TexCoord.y * 2 - 1;

	vec4 vProjectedPos = vec4(x, y, z, 1.0f);
	vec4 sPos = invProj * vProjectedPos;

	return (sPos.xyz / sPos.w);
}

vec3 FindBlocker(vec3 coord, int useCascade, float offset)
{
	float searchWidth = 2 * offset;
	//float searchWidth = 
	float blockersum = 0;
	float numblockers = 0;

	int i;
	for(i = 0; i < 16; i++)
	{
		float shadowMapDepth = texture(g_ShadowDepth, vec3(coord.xy + poissonDisk[i] * searchWidth, useCascade));
		if(shadowMapDepth < coord.z)
		{
			blockersum += shadowMapDepth;
			numblockers += 1;
		}
	}

	float avgBlockerDepth = blockersum / numblockers;
	return vec3(avgBlockerDepth, numblockers, searchWidth);
}

float PenumbraSize(float lightspaceFragDepth, float blockerDepth)
{
	return (lightspaceFragDepth - blockerDepth) / blockerDepth;
}

float PCF_Filter(vec3 coord, int useCascade, float filterRadiusUV)
{
	float sum = 0.0;
	int i;
	for(i = 0; i < PCF_NUM_SAMPLES; ++i)
	{
		vec2 offset = poissonDisk[i] * filterRadiusUV;
		sum += texture(g_ShadowDepthPCF, vec4(coord.xy + offset, useCascade, coord.z));
	}
	return sum / PCF_NUM_SAMPLES;
}

float PCSS(vec3 coord, float fragmentZLightSpace, int useCascade)
{
	vec4 penumbraSize = vec4(0.1, 0.0, 0.0, 0.0);
	penumbraSize = shadowCasterViewProjectionMatrix[useCascade] * penumbraSize;
	float offset = length(penumbraSize);
	vec3 blockerData = FindBlocker(vec3(coord.xy, coord.z + 0.0001), useCascade, offset); //x is avgBlockerDepth, y is numblockers
	if(blockerData.y < 1)
	{
		return 1.0; //Early out since no blockers
	}

	float penumbraRatio = PenumbraSize(coord.z, blockerData.x);
	float filterRadiusUV = min(blockerData.z, offset*0.2 + (penumbraRatio * 700000 * offset / fragmentZLightSpace)); //Constant should be tweaked


	//return blockerData.y / 16;
	return PCF_Filter(coord, useCascade, filterRadiusUV);
}

void main() {

	vec4 rt0 = texture(g_Diffuse, ex_TexCoord);

	vec3 diffuse = rt0.xyz;
	float specTerm = rt0.w;

	vec2 vert_normal = texture(g_Normals, ex_TexCoord).xy;
	
	vec3 normal;
	normal.xy = vert_normal.xy;
    normal.z = sqrt(1-dot(normal.xy, normal.xy));

	vec3 position = GetVSPositionFromDepth();

	vec4 worldPosition = invView * vec4(position, 1.0);
	vec4 shadowCoord;
	vec3 colors[4];
	colors[0] = vec3(1.0, 0.0, 0.0);
	colors[1] = vec3(0.0, 1.0, 0.0);
	colors[2] = vec3(0.0, 0.0, 1.0);
	colors[3] = vec3(0.0, 1.0, 1.0);
	float offsets[4];
	int cascade = 0;
	float visibility = 0.0f;
	int useCascade;

	for(cascade = 0; cascade < 4; cascade++)
	{
		shadowCoord = shadowCasterViewProjectionMatrix[cascade] * worldPosition;
		if(cascade == 3)
		{

			useCascade = cascade;
			break;
		}
		if(max(abs(shadowCoord.x - 0.5), abs(shadowCoord.y - 0.5)) < 0.48)
		{
			if(shadowCoord.z < 1)
			{
				useCascade = cascade;
				break;
			}
		}
	}


	float occluderDepth = texture(g_ShadowDepth, vec3(shadowCoord.xy, useCascade));
	occluderDepth = shadowCoord.z - occluderDepth;
	

	float distanceFromFragmentToLight = length(position - sunPosition);

	visibility = PCSS(shadowCoord.xyz, distanceFromFragmentToLight, useCascade);

	
	//shadowCoord /= shadowCoord.w; //unnecessary because orthographic

	vec3 vert_lightVec = normalize( -ex_Light.LightDirection );

	vec3 viewDir = -normalize(position);
	vec3 halfVector = normalize(viewDir + vert_lightVec);

	vec3 spec_color = vec3(specTerm) * pow(clamp(dot(normal, halfVector), 0.0, 1.0), 128.0f);
	vec3 diffuse_color = diffuse * max( 0.0f, dot( normalize( vert_lightVec ), normal ) ) * ex_Light.Color.xyz;

	out_Color = vec4(diffuse_color + spec_color, 1.0);

	if(shadowCoord.x <= 0 || shadowCoord.x >= 1 || shadowCoord.y <= 0 || shadowCoord.y >= 1)
	{
		visibility = 0;
	}

	out_Color *= visibility;
}