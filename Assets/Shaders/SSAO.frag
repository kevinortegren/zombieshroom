#version 430

in vec2 ex_TexCoord;
out vec4 out_Color;

uniform sampler2D g_Normals;
uniform sampler2D g_Depth;
uniform sampler2D g_Noise;

const float uRadius = 1.0;
const float uPower = 1.0;
const int uSampleKernelSize = 16;

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
	vec4 uSampleKernel[16]; 	
};

vec3 GetVSPositionFromDepth(vec2 sampleCoord)
{
	float z = texture(g_Depth, sampleCoord).r;

	z = z * 2 - 1;

	float x = sampleCoord.x * 2 - 1;
	float y = sampleCoord.y * 2 - 1;

	vec4 vProjectedPos = vec4(x, y, z, 1.0);
	vec4 sPos = invProj * vProjectedPos;

	return (sPos.xyz / sPos.w);
}

float linearizeDepth(in float depth) {
	return projectionMatrix[3][2] / (depth - projectionMatrix[2][2]);
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
	vec2 texCoord = vec2(gl_FragCoord.st)/textureSize(g_Depth, 0);
	vec3 origin = GetVSPositionFromDepth(texCoord);
	vec3 normal = GetDecodedNormal(texCoord);
    //normal = normalize(normal);

	//normal *= 2.0 - 1.0;

	vec3 rvec = texture(g_Noise, texCoord * textureSize(g_Depth, 0) * 0.25).xyz * 2.0 - 1.0;
	vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 tbn = mat3(tangent, bitangent, normal);

	float sampleDepth = 0;
	vec3 samp;
	float occlusion = 0.0;
	for (int i = 0; i < uSampleKernelSize; ++i) 
	{
		// get sample position:
		samp = tbn * uSampleKernel[i].xyz;
		samp = samp * uRadius + origin;

		// project sample position:
		vec4 offset = vec4(samp, 1.0);
		offset = projectionMatrix * offset;
		offset.xy /= offset.w;
		offset.xy = offset.xy * 0.5 + 0.5;

		// get sample depth:
		sampleDepth = GetVSPositionFromDepth(offset.xy).z;

		// range check & accumulate:
		float rangeCheck = abs(origin.z - sampleDepth) < uRadius ? 1.0 : 0.0;
		///float rangeCheck = smoothstep(0.0, 1.0, uRadius / abs(origin.z - sampleDepth));
		occlusion += (sampleDepth > samp.z ? 1.0 : 0.0) * rangeCheck;
		//occlusion += rangeCheck * step(sampleDepth, samp.z);
	}

	occlusion = 1 - (occlusion / float(uSampleKernelSize));
	occlusion = pow(occlusion, uPower);
    out_Color = vec4(occlusion);

	//out_Color = vec4(vec3(errtest), 1.0);
	//out_Color = vec4(1.0, 0.0, 0.0, 1.0);
}