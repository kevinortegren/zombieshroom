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
	mat4 shadowCasterViewProjectionMatrix;
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
uniform sampler2DShadow g_ShadowDepth;

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

	vec3 vert_normal = texture(g_Normals, ex_TexCoord).xyz;
	
	vec3 normal = normalize(vert_normal.xyz*2-1); 
	vec3 position = GetVSPositionFromDepth();

	vec4 worldPosition = invView * vec4(position, 1.0);

	mat4 viewToLightViewProjection = shadowCasterViewProjectionMatrix * invView;
	vec4 shadowCoord = viewToLightViewProjection * vec4(position, 1.0);
	shadowCoord = shadowCasterViewProjectionMatrix * worldPosition;
	
	//shadowCoord /= shadowCoord.w; //unnecessary because orthographic

	vec3 vert_lightVec = normalize( -ex_Light.LightDirection );

	vec3 viewDir = -normalize(position);
	vec3 halfVector = normalize(viewDir + vert_lightVec);

	vec3 spec_color = vec3(specTerm) * pow(clamp(dot(normal, halfVector), 0.0, 1.0), 128.0);
	vec3 diffuse_color = diffuse * max( 0.0f, dot( normalize( vert_lightVec ), normal ) ) * ex_Light.Color.xyz;

	out_Color = vec4(diffuse_color + spec_color, 1.0);

	float visibility = texture(g_ShadowDepth, vec3(shadowCoord.xy, shadowCoord.z));

	if(shadowCoord.x <= 0 || shadowCoord.x >= 1 || shadowCoord.y <= 0 || shadowCoord.y >= 1)
	{
		visibility = 1;
	}

	out_Color *= visibility;
}