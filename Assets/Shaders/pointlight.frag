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
uniform sampler2D g_GlowTrans;

out vec4 out_Color;

vec3 GetVSPositionFromDepth(vec2 texcoord)
{
	float z = texture(g_Depth, texcoord).r;

	if(z == 1)
		discard;
		
	z = z * 2 - 1;

	float x = texcoord.x * 2 - 1;
	float y = texcoord.y * 2 - 1;

	vec4 vProjectedPos = vec4(x, y, z, 1.0);
	vec4 sPos = invProj * vProjectedPos;

	return (sPos.xyz / sPos.w);
}

vec3 GetDecodedNormal(vec2 TexCoord)
{
	vec2 vert_normal = texture(g_Normals, TexCoord).xy;	    
    vec2 fenc = vert_normal*4-2;
    float f = dot(fenc,fenc);
    float g = sqrt(1-f/4);
    vec3 normal;
    normal.xy = fenc*g;
    normal.z = 1-f/2;
    return normal;
}

void main() {

    vec2 TexCoord = vec2(gl_FragCoord.st)/textureSize(g_Diffuse, 0);
	float translucency = texture(g_GlowTrans, TexCoord).a;

    // Position.
    vec3 position = GetVSPositionFromDepth(TexCoord);

    // Direction, Distance
    vec3 vert_lightVec = ex_Light.LightPosition - position;
    float dist = length(vert_lightVec);

	vec3 light = vec3(0.0);
	
	// Normal.
	vec3 normal = GetDecodedNormal(TexCoord);
  
	// Materials.
	vec4 rt0 = texture(g_Diffuse, TexCoord);
	vec3 diffuse = rt0.xyz;
	float specTerm = rt0.w;

	vert_lightVec = normalize(vert_lightVec);

	vec3 viewDir = -normalize(position);
	vec3 halfVector = normalize(viewDir + vert_lightVec);

	//Translucency
	float EdotL = max(0.0, dot(-viewDir, vert_lightVec)); // 1 if facing the light.
    float LdotN = max(0.0, dot( vert_lightVec, -normal)); // 1 if inverse normal facing the light.  

    float transFactor = clamp(EdotL * LdotN, 0.0, 1.0) * translucency;
    float diffuseFactor = max(0.0, dot( normalize( vert_lightVec ), normal ));
    
	vec3 spec_color = ex_Light.Color.xyz * specTerm * pow(clamp(dot(normal, halfVector), 0.0, 1.0), 128.0);
	vec3 diffuse_color = clamp(diffuseFactor + transFactor, 0.0, 1.0) * diffuse * ex_Light.Color.xyz;

	light = diffuse_color + spec_color;
	light = light / dot(ex_Light.Attenuation, vec3(1, dist, dist*dist));     
	
    out_Color = vec4(light, 1.0);
}