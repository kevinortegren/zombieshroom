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

// GBuffer.
uniform sampler2D g_Diffuse;
uniform sampler2D g_Normals;
uniform sampler2D g_Depth;
uniform sampler2DArrayShadow g_ShadowDepth;

out vec4 out_Color;

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
	int cascade = 0;
	int useCascade;
	for(cascade = 0; cascade < 4; cascade++)
	{
		shadowCoord = shadowCasterViewProjectionMatrix[cascade] * worldPosition;
		if(max(abs(shadowCoord.x - 0.5), abs(shadowCoord.y - 0.5)) < 0.48)
		{
			if(shadowCoord.z < 1)
			{
				useCascade = cascade;
				break;
			}
		}
	}

	
	//shadowCoord /= shadowCoord.w; //unnecessary because orthographic

	vec3 vert_lightVec = normalize( -ex_Light.LightDirection );

	vec3 viewDir = -normalize(position);
	vec3 halfVector = normalize(viewDir + vert_lightVec);

	vec3 spec_color = vec3(specTerm) * pow(clamp(dot(normal, halfVector), 0.0, 1.0), 128.0f);
	vec3 diffuse_color = diffuse * max( 0.0f, dot( normalize( vert_lightVec ), normal ) ) * ex_Light.Color.xyz;

	out_Color = vec4(diffuse_color + spec_color, 1.0);

	float visibility = texture(g_ShadowDepth, vec4(shadowCoord.xy, useCascade, shadowCoord.z));

	if(shadowCoord.x <= 0 || shadowCoord.x >= 1 || shadowCoord.y <= 0 || shadowCoord.y >= 1)
	{
		visibility = 0;
	}

	out_Color *= visibility;
}