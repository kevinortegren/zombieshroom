#version 410 core 

in vec3 WorldPos_FS_in;                                                                        
in vec2 TexCoord_FS_in;                                                                        
in vec3 Normal_FS_in; 

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec3 normals;
layout (location = 2) out vec4 glow;
layout (location = 3) out vec4 background;

uniform samplerCube g_Specular;
//uniform samplerCube CubeMap;
uniform sampler2D g_LA;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};  

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
	mat4 normalMatrix;
	vec3 gEyeWorldPos;
};    

void main()
{
	vec2 screenTexCoord = gl_FragCoord.xy / textureSize(g_LA, 0);
    vec3 la = texture(g_LA, screenTexCoord).xyz;

	vec3 viewNormal = normalize(viewMatrix*vec4(Normal_FS_in,0.0f)).xyz;

	//Reflection calculations
	vec3 incident	= WorldPos_FS_in - gEyeWorldPos;
	vec3 refW		= reflect(incident, normalize(Normal_FS_in));
	vec3 frag_color = texture(g_Specular, refW).xyz;
	 
	vec3 trans = la + frag_color.rgb*0.3f;
	diffuse = vec4(vec3(0.0f, 0.0f, 0.1f), 0.6f);
	normals = vec3(viewNormal * 0.5 + 0.5);
	glow = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	background = vec4(trans, 0.3f);
}