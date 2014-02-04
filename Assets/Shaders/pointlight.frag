#version 400

struct PointLight
{
	vec3 LightPosition;
	vec3 Attenuation;
	float Range;
	vec4 Color;
};

in vec2 ex_TexCoord;
in PointLight ex_Light;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 invView;
	mat4 invProj;
	mat4 invViewProj;
};

// GBuffer.
uniform sampler2D g_Diffuse;
uniform sampler2D g_Normals;
uniform sampler2D g_Depth;

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

	vec3 vert_lightVec = ex_Light.LightPosition - position;
	float dist = length(vert_lightVec);

	vert_lightVec = normalize(vert_lightVec);

	vec3 viewDir = -normalize(position);
	vec3 halfVector = normalize(viewDir + vert_lightVec);

	vec3 light = vec3(0);
	if(dist <= ex_Light.Range)
	{
		vec3 spec_color = vec3(specTerm) * pow(clamp(dot(normal, halfVector), 0.0f, 1.0f), 128.0f);
		vec3 diffuse_color = diffuse * max( 0.0f, dot( normalize( vert_lightVec ), normal ) ) * ex_Light.Color.xyz;

		light = diffuse_color + spec_color;
		light = light / dot(ex_Light.Attenuation, vec3(1, dist, dist*dist));
	}

	out_Color = vec4(light, 1.0f);
}