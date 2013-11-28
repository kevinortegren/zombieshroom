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
	float z = texture(g_Depth, ex_TexCoord).x;
	float x = ex_TexCoord.x * 2 - 1;
    float y = (1 - ex_TexCoord.y) * 2 - 1;

	vec4 vProjectedPos = vec4(x, y, z, 1.0f);
    vec4 sPos = invViewProj * vProjectedPos;

    return (sPos.xyz / sPos.w);
}

void main() {

	//vec3 diffuse = texture(g_Diffuse, ex_TexCoord).xyz;
	vec3 vert_normal = texture(g_Normals, ex_TexCoord).xyz;

	vec3 normal = normalize(vert_normal.xyz*2-1); 
	vec3 position = GetVSPositionFromDepth();

	vec3 vert_lightVec = position - ex_Light.LightPosition;
	float dist = length(vert_lightVec);

	vec3 light = vec3(0);
	if(dist < ex_Light.Range)
	{
		float LdotN = max( 0.0f, dot( normalize( -vert_lightVec ), normal ) );
		light = LdotN * ex_Light.Color.xyz;
		light = light / dot(ex_Light.Attenuation, vec3(1, dist, dist*dist));
	}

    //out_Color = vec4(dist * 0.1f, 0, 0, 1.0f);
    //out_Color = vec4(ex_Light.LightPosition, 1.0f);

    out_Color = vec4(light, 1.0f);
}