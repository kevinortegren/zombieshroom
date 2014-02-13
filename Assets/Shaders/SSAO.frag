#version 400

in vec2 ex_TexCoord;
out vec4 out_Color;

uniform sampler2D g_Normals;
uniform sampler2D g_Depth;
uniform sampler2D g_Noise;

uniform float uRadius = 1.5;
uniform float uPower = 2.0;
uniform int uSampleKernelSize = 16;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 invView;
	mat4 invProj;
	mat4 invViewProj;
};

layout(std140) uniform PerTech
{
	vec3 uSampleKernel[16]; 	
};

vec3 GetVSPositionFromDepth(vec2 sampleCoord)
{
	float z = texture(g_Depth, sampleCoord).r;

	if(z == 1)
	discard;
  
	z = z * 2 - 1;

	float x = sampleCoord.x * 2 - 1;
	float y = sampleCoord.y * 2 - 1;

	vec4 vProjectedPos = vec4(x, y, z, 1.0f);
	vec4 sPos = invProj * vProjectedPos;

	return (sPos.xyz / sPos.w);
}

float linearizeDepth(in float depth) {
	return projectionMatrix[3][2] / (depth - projectionMatrix[2][2]);
}

void main() {
	vec3 origin = GetVSPositionFromDepth(ex_TexCoord);
	vec2 vert_normal = texture(g_Normals, ex_TexCoord).xy;
	vec3 normal;
	normal.xy = vert_normal.xy;
    normal.z = sqrt(1-dot(normal.xy, normal.xy));
    normal = normalize(normal);

	vec3 rvec = vec3(0,1,0);//texture(g_Noise, ex_TexCoord).xyz * 2.0 - 1.0;
	vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 tbn = mat3(tangent, bitangent, normal);

	float occlusion = 0.0;
	for (int i = 0; i < uSampleKernelSize; ++i) 
	{
		// get sample position:
		vec3 samp = tbn * uSampleKernel[i];
		samp = samp * uRadius + origin;

		// project sample position:
		vec4 offset = vec4(samp, 1.0);
		offset = projectionMatrix * offset;
		offset.xy /= offset.w;
		offset.xy = offset.xy * 0.5 + 0.5;

		// get sample depth:
		float sampleDepth = GetVSPositionFromDepth(offset.xy);

		// range check & accumulate:
		float rangeCheck= abs(origin.z - sampleDepth) < uRadius ? 1.0 : 0.0;
		occlusion += (sampleDepth <= samp.z ? 1.0 : 0.0) * rangeCheck;
	}

	occlusion = 1.0 - (occlusion / uSampleKernelSize);
	occlusion = pow(occlusion, uPower);
    out_Color = vec4(vert_normal,0, 1);
}