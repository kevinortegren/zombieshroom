#version 400

in vec3 vert_texcoord;


layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec3 normals;
layout (location = 2) out vec4 glow;
layout (location = 3) out vec4 background;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
};

layout(std140) uniform PerTech
{
	vec4 sunDirection;
	//vec4 sunColor;
};

//const vec3 sunDirection = -vec3(-0.115862, -0.993221, -0.00935733);
const vec3 sunColor = vec3(1.0, 1.0, 0.90);


//(-0.115862 | -0.993221 | -0.00935733
//-0.972269 | 0.115335 | -0.203448
//0.203148 | -0.014474 | -0.979041)


void main()
{
	vec3 frag_color = vec3(0.0, 0.0, 0.0);

	vec3 blue2 = vec3(74.0/256.0, 120.0/256.0, 172.0/256.0);
	vec3 blue = vec3(149.0/256.0, 183.0/256.0, 211.0/256.0);

	float d = abs(dot(vec3(0.0, 1.0, 0.0), normalize(vert_texcoord)));
	frag_color = vec3(mix(blue.r, blue2.r, d), mix(blue.g, blue2.g, d), mix(blue.g, blue2.g, d));

	vec3 worldFragCoord = (modelMatrix * vec4(normalize(vert_texcoord), 0.0)).xyz;
	float s = max(0.0, dot(normalize(sunDirection.xyz), normalize(worldFragCoord)));
	
	float sunFactor = 0.0;
	sunFactor = pow(s, 256.0) * 0.7;
	vec3 sunContribution = vec3(0.0);
	if(s > 0.999)
	{
		sunFactor = 1.0;
		sunContribution = sunColor * sunFactor;
	}
	frag_color += sunColor * sunFactor;

    diffuse = vec4(vec3(0), 0);
	normals = vec3(0);
	glow = vec4(sunContribution, 0.0);
	background = vec4(frag_color, 1.0);
}